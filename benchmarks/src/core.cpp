#include <benchmarks/core.hpp>

std::string formatTime(double time) {
	// Time is in nanoseconds
	std::stringstream ss;
	ss << std::fixed << std::setprecision(6) << (time / 1000000) << "ms";
	return ss.str();
}

bool jsonContains(const json::json &j, const std::string &key) { return j.find(key) != j.end(); }

std::string tableToString(const json::json &table) {
	std::string tableString("-+-");
	std::vector<int64_t> colWidths;
	int64_t maxRowLength = 0;

	// Extract column widths
	for (const auto &[key, value] : table.items()) {
		size_t maxWidth = key.length();
		for (const auto &val : value) {
			std::stringstream stream;
			stream << val;
			maxWidth = lrc::max(maxWidth, static_cast<int64_t>(stream.str().length()));
		}
		colWidths.emplace_back(maxWidth);
		tableString += std::string(maxWidth, '-') + "-+-";
	}

	// Extract row lengths
	for (const auto &[key, value] : table.items()) {
		maxRowLength = lrc::max(maxRowLength, int64_t(value.size()));
	}

	std::string heading("\n | ");
	std::string line("-+-");
	int64_t index = 0;
	for (const auto &[key, value] : table.items()) {
		std::stringstream stream;
		stream << std::setw(colWidths[index]) << std::left << key;
		heading += stream.str();
		line += std::string(colWidths[index], '-');
		heading += " | ";
		line += "-+-";
		++index;
	}

	tableString += fmt::format("{}\n{}", heading, line);

	std::vector<std::vector<std::string>> rows(maxRowLength);

	// Extract rows
	int64_t colIndex = 0;
	for (const auto &[key, value] : table.items()) {
		for (int64_t i = 0; i < maxRowLength; ++i) {
			std::stringstream stream;
			stream << std::setw(colWidths[colIndex]) << std::left
				   << (i < value.size() ? value[i] : json::json {});
			rows[i].emplace_back(stream.str());
		}
		colIndex++;
	}

	for (int64_t i = 0; i < maxRowLength; ++i) {
		std::string rowString(" | ");
		for (int64_t j = 0; j < colWidths.size(); ++j) {
			rowString += fmt::format("{:>{}}", rows[i][j], colWidths[j]);
			rowString += " | ";
		}
		tableString += fmt::format("\n{}", rowString);
	}

	// Add "---" at the end
	tableString += "\n-+-";
	index = 0;
	for (const auto &[key, value] : table.items()) {
		std::stringstream stream;
		tableString += std::string(colWidths[index], '-');
		tableString += "-+-";
		++index;
	}

	return tableString;
}
