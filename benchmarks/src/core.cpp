#include <benchmarks/core.hpp>

std::string formatTime(double time) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(6) << time;
	return ss.str();
}

bool jsonContains(const json::json &j, const std::string &key) { return j.find(key) != j.end(); }

std::string tableToString(const json::json &table) {
	std::string tableString("-+-");
	std::vector<lrc::i64> colWidths;
	lrc::i64 maxRowLength = 0;

	// Extract column widths
	for (const auto &[key, value] : table.items()) {
		size_t maxWidth = key.length();
		for (const auto &val : value) {
			std::stringstream stream;
			stream << val;
			maxWidth = lrc::max(maxWidth, static_cast<lrc::i64>(stream.str().length()));
		}
		colWidths.emplace_back(maxWidth);
		tableString += std::string(maxWidth, '-') + "-+-";
	}

	// Extract row lengths
	for (const auto &[key, value] : table.items()) {
		maxRowLength = lrc::max(maxRowLength, lrc::i64(value.size()));
	}

	std::string heading("\n | ");
	std::string line("-+-");
	lrc::i64 index = 0;
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
	lrc::i64 colIndex = 0;
	for (const auto &[key, value] : table.items()) {
		for (lrc::i64 i = 0; i < maxRowLength; ++i) {
			std::stringstream stream;
			stream << std::setw(colWidths[colIndex]) << std::left
				   << (i < value.size() ? value[i] : json::json {});
			rows[i].emplace_back(stream.str());
		}
		colIndex++;
	}

	for (lrc::i64 i = 0; i < maxRowLength; ++i) {
		std::string rowString(" | ");
		for (lrc::i64 j = 0; j < colWidths.size(); ++j) {
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
