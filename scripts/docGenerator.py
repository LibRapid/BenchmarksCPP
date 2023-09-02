import os
import argparse
import colorama
import shutil

EXCLUDE_DIRS = [
    "documentation",
    "sphinx"
]

# Set up the argument parser
parser = argparse.ArgumentParser(description='Run the main program.')

# Add the arguments
parser.add_argument("-i", "--input", help="Input Directory", required=False, type=str, dest="input", default="TestResults")
parser.add_argument("-o", "--output", help="Output Directory", required=False, type=str, dest="output", default="BenchmarkResults")

args = parser.parse_args()


def printLog(*args, **kwargs):
    print(colorama.Fore.CYAN, end="")
    print(*args, **kwargs)
    print(colorama.Style.RESET_ALL, end="")


def printWarning(*args, **kwargs):
    print(colorama.Fore.YELLOW + colorama.Style.BRIGHT, end="")
    print(*args, **kwargs)
    print(colorama.Style.RESET_ALL, end="")


def printError(*args, **kwargs):
    print(colorama.Fore.RED + colorama.Style.BRIGHT, end="")
    print(*args, **kwargs)
    print(colorama.Style.RESET_ALL, end="")


def ensureDirectoryExists(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)


def ensureFileExists(filename):
    if not os.path.exists(filename):
        open(filename, "a").close()


def loadWarning():
    with open("helpers/warning.md", "r") as f:
        return f.read()


def loadBenchmarkPreamble():
    with open("helpers/benchmarkPreamble.md", "r") as f:
        return f.read()


def parseDirectoryName(dirName):
    # Image directory names are of the form: "LibRapid_<OperatingSystem>_<Compiler>(_<SmallArrays>?)

    out = dirName.split("_")
    if len(out) == 3:
        return {
            "os": out[1],
            "compiler": out[2],
            "smallArrays": False
        }
    elif len(out) == 4:
        return {
            "os": out[1],
            "compiler": out[2],
            "smallArrays": True
        }
    else:
        # Invalid directory name (probably not a benchmark output)
        return None


def parseFileName(filename):
    # File names have the format: "bench_<Operation Name>_<Num Threads>_threads.png"

    out = filename.split("_")
    if len(out) == 4:
        return {
            "file": filename,
            "operation": out[1],
            "numThreads": out[2]
        }


def generateMarkdownToctree(title, items):
    nlItems = "\n".join(items)

    return f"""
# {title}

```{{toctree}}
:maxdepth: 4

{nlItems}
```
"""


def generateMarkdownForFile(fileInfo, showTitle=True):
    title = f"{fileInfo['numThreads']} {'thread' if fileInfo['numThreads'] == '1' else 'threads'}"
    warning = loadWarning()
    image = f"![{fileInfo['file']}]({fileInfo['file']})"

    return f"""
{f'## {title}' if showTitle else "## (Optimised for Small Arrays)"}

{warning}

{image}
"""


def generateMarkdownListings(rootDir, recurse=True, depth=0):
    rootName = os.path.basename(rootDir)

    subDirs = []
    mdFiles = []

    for file in os.listdir(rootDir):
        if file.endswith(".md"):
            mdFiles.append(file)
        if os.path.isdir(os.path.join(rootDir, file)):
            subDirs.append(file)

    if recurse and len(subDirs) > 0:
        for subDir in subDirs:
            generateMarkdownListings(os.path.join(rootDir, subDir), recurse=True, depth=depth + 1)

        generateMarkdownListings(rootDir, recurse=False, depth=depth)
        return

    print(f"Writing {rootDir + '.md'}")
    with open(rootDir + ".md", "w") as file:
        mdLinks = [f"{rootName}/{file}" for file in mdFiles]
        if depth == 1:
            file.write(loadBenchmarkPreamble())
            file.write(generateMarkdownToctree("Operating Systems", mdLinks))
        else:
            file.write(generateMarkdownToctree(rootName, mdLinks))


def shouldSkip(directory):
    rootName = os.path.basename(directory)
    return rootName.lower() in EXCLUDE_DIRS


printLog(f"Input Directory: {args.input}")
printLog(f"Output Directory: {args.output}")

for root, dirs, files in os.walk(args.input):
    # print(f"Root: {root}, Dirs: {dirs}, Files: {files}")
    dirInfo = parseDirectoryName(root)
    if dirInfo is not None and not shouldSkip(root):
        outputDirectoryName = os.path.join(os.path.join(args.output, "BenchmarkResults"),
                                           dirInfo["os"],
                                           dirInfo["compiler"],
                                           "OPTIMISE_SMALL_ARRAYS=ON" if dirInfo["smallArrays"] else "OPTIMISE_SMALL_ARRAYS=OFF")
        ensureDirectoryExists(outputDirectoryName)

        for file in files:
            fileInfo = parseFileName(file)
            if fileInfo is None:
                printError(f"Invalid Directory: {root}/{file}")
                continue

            outputFileName = os.path.join(outputDirectoryName, fileInfo["operation"]) + ".md"
            ensureFileExists(outputFileName)

            # Copy file to output directory
            shutil.copy(os.path.join(root, file), os.path.join(outputDirectoryName, file))

            print(f"Writing {outputFileName}")
            with open(outputFileName, "a") as f:
                f.write(f"# {fileInfo['operation']}")
                f.write(generateMarkdownForFile(fileInfo, showTitle=not dirInfo["smallArrays"]))
    else:
        printWarning(f"Invalid Directory: {root}")

generateMarkdownListings(args.output)
