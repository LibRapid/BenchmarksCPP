import argparse
import pandas
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
import os

# Set up the argument parser
parser = argparse.ArgumentParser(description="Process data from a benchmark run.")

parser.add_argument("-i", "--input", help="The input file to process (CSV).", required=False)
parser.add_argument("-o", "--output", help="The output file to write to.", required=False)
parser.add_argument("-r", "--relative", help="Plot results relative to a given library.", required=False)
parser.add_argument("-a", "--absolute", help="Plot results absolute to a given library.", required=False)
parser.add_argument("-l", "--log", help="Plot results on a logarithmic scale.", action="store_true", required=False)
parser.add_argument("-s", "--show", help="Show the plots after processing.", action="store_true", required=False)
parser.add_argument("-v", "--verbose", help="Show verbose output.", action="store_true", required=False)

args = parser.parse_args()

inputFile = None  # The input file to process
outputDir = None  # The output file to write to
graphingMode = None  # The mode to use for graphing (relative or absolute)
showGraphs = None  # Whether to show the graphs after processing
relativeTo = None  # The library to use as a reference for relative graphing
verbose = None  # Whether to show verbose output
logPlot = None  # Whether to plot the results on a logarithmic scale

if args.input is None:
    print("No input file specified.")
    exit(1)
else:
    inputFile = args.input

if args.output is not None:
    outputDir = args.output

if args.output is None and not args.show:
    print("No output mode specified. Please specify either --output or --show")
    exit(1)

if args.relative is not None and args.absolute is not None:
    print("Please specify either --relative or --absolute, not both.")
    exit(1)

if args.relative is None and args.absolute is None:
    graphingMode = "relative"

if args.relative is not None:
    graphingMode = "relative"
    relativeTo = args.relative

if args.absolute is not None:
    graphingMode = "absolute"
    relativeTo = args.absolute

logPlot = args.log
showGraphs = args.show
verbose = args.verbose


def verbosePrint(*args, **kwargs):
    if verbose:
        print(*args, **kwargs)


def readFile(path=inputFile, delimiter=";"):
    """
    Read the input file and return the data as a pandas DataFrame.
    :param path: The path to the input file.
    :return: A pandas DataFrame containing the data.
    """

    with open(path, "r") as f:
        verbosePrint("Reading file...")
        data = pandas.read_csv(f, delimiter=';')

    verbosePrint("File read successfully.")
    return data


def extractRuns(data):
    """
    Extract the runs from the data.
    :param data: The data to extract the runs from.
    :param relativeTo_: The library to use as a reference for relative graphing.
    :return: A dictionary containing the runs.
    """

    result = []
    opName = None
    for index, row in data.iterrows():
        verbosePrint("Extracting run: " + row["name"])

        name = row["name"]  # Name is constructed as follows: "<Library> | <Operation> | <Size> | <Threads>"
        library, operation, size, threads = name.split(" | ")

        if opName is None:
            opName = operation

        if opName != operation:
            print("Error: operation names do not match. Expected: " + opName + ", got: " + operation)
            exit(1)

        result.append({
            "library": library,
            "operation": operation,
            "size": size,
            "threads": threads,
            "elapsed": row["elapsed"],
            "op/s": 1 / row["elapsed"],
            "error": row["error %"],
            "instructions": row["instructions"],
            "branches": row["branches"],
            "branchMisses": row["branch misses"],
        })

        verbosePrint("Extracted run: " + row["name"])

    return result


def processRuns(runs, graphingMode_=graphingMode, relativeTo_=relativeTo):
    """
    Process the data in the runs.
    :param runs: The runs to process.
    :param graphingMode_: The mode to use for graphing (relative or absolute).
    :param relativeTo_: The library to use as a reference for relative graphing.
    :return: A list of processed runs.
    """

    result = []

    for run in runs:
        verbosePrint("Processing run: " + run["library"] + " | " + run["operation"] + " | " + run["size"] + " | " + run["threads"])
        if graphingMode_ == "absolute":
            result.append(run)
        elif graphingMode_ == "relative":
            if run["library"] == relativeTo_:
                result.append({
                    "library": run["library"],
                    "operation": run["operation"],
                    "size": run["size"],
                    "threads": run["threads"],
                    "elapsed": 1,
                    "op/s": 1,
                    "error": run["error"],
                    "instructions": 1,
                    "branches": 1,
                    "branchMisses": 1
                })
            else:
                if relativeTo_ is None:
                    print("Error: no library specified for relative graphing.")
                    exit(1)

                relativeRun = next(filter(lambda x: x["library"] == relativeTo_ and
                                                    x["operation"] == run["operation"] and
                                                    x["size"] == run["size"] and
                                                    x["threads"] == run["threads"], runs))

                def relativeHelper(name):
                    if relativeRun[name] == 0:
                        return 0
                    else:
                        return run[name] / relativeRun[name]

                result.append({
                    "library": run["library"],
                    "operation": run["operation"],
                    "size": run["size"],
                    "threads": run["threads"],
                    "elapsed": relativeHelper("elapsed"),
                    "op/s": relativeHelper("op/s"),
                    "error": run["error"],
                    "instructions": relativeHelper("instructions"),
                    "branches": relativeHelper("branches"),
                    "branchMisses": relativeHelper("branchMisses")
                })
        else:
            print("Error: invalid graphing mode specified.")
            exit(1)

        verbosePrint("Processed run: " + run["library"] + " | " + run["operation"] + " | " + run["size"] + " | " + run["threads"])

    return result


def generateGraphs(data, showGraphs_=showGraphs, outputDir_=outputDir):
    # Split the data by the number of threads used
    threadSet = set([element["threads"] for element in data])

    for threads in threadSet:
        verbosePrint("Generating graph for " + threads + " threads...")

        runs = list(filter(lambda x: x["threads"] == threads, data))

        xVals = []
        yVals = []
        errors = []
        for run in runs:
            xVals.append(f"{run['library']} | {run['size']}")
            yVals.append(run["op/s"])
            errors.append(run["error"] * run["op/s"])

        # Insert empty elements to separate sizes
        step = len(set([run["library"] for run in runs]))
        index = len(xVals) - step
        while index >= 0:
            xVals.insert(index, " " * index)
            yVals.insert(index, 0)
            errors.insert(index, 0)
            index -= step

        # Plot the data
        colors = cm.tab10(np.linspace(0, 1, step + 1))

        plt.figure(figsize=(10, 10))
        plt.bar(xVals, yVals, color=colors, align="center")
        plt.errorbar(xVals, yVals, yerr=errors, fmt="none", ecolor="red", capsize=3)
        plt.xticks(rotation=90)
        plt.ylabel("Operations/Second" + (" (Relative)" if graphingMode == "relative" else ""))
        plt.xlabel("Library | Size")

        if logPlot:
            plt.yscale("log")

        plt.tight_layout()

        if outputDir_ is not None:
            if not os.path.exists(outputDir_):
                verbosePrint("Output directory does not exist, creating it...")
                os.makedirs(outputDir_)

            verbosePrint("Saving graph for " + threads + " threads...")
            plt.savefig(f"{outputDir_}/bench_{threads}.png")

        if showGraphs_:
            verbosePrint("Showing graph for " + threads + " threads...")
            plt.show()


data = readFile()
runs = extractRuns(data)
processed = processRuns(runs)
generateGraphs(processed)
