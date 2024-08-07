/**
 * @file Format.cpp
 * @author Ahmed Khaled
 * @brief This file implements functions in Format.h
 */

#include <string>
#include <regex>

#include "Format.h"
#include "Config.h"
#include "Enums.h"
#include "Utils.h"

using namespace std;

extern Config config;

/**
 * @brief Indents (puts 4 spaces) before all lines in a string
 * @param s The input string
 * @return The indented string
 */
string indentAllLinesInString(string s) {
    bool isNewLine = 1;
    string result;
    for (char c : s) {
        if (isNewLine) {
            result += "    ";
        }
        result += c;

        isNewLine = (c == '\n');
    }

    return result;
}

/**
 * @brief Replaces all plain text hash ids (issue ids and pull request ids (#2777)) with links to these issues/pull requests on GitHub
 * @param pullRequestBody The original body/description of the pull request to do the replacements on
 * @return Pull request body/description after performing the replacements
 */
string replaceHashIdsWithLinks(string pullRequestBody) {
    // The first brackets are not considered a capture group, they are a must when adding "R" to define this as a *raw string literal*
    // We add this "R" to write regex patterns easier and increase readability by not needing to escape back slashes
    regex hashIdPattern(R"(#(\d+))");

    string result = pullRequestBody;
    size_t numberOfNewCharactersAdded = 0;
    sregex_iterator firstHashId(pullRequestBody.begin(), pullRequestBody.end(), hashIdPattern);
    // This is an end of sequence iterator
    sregex_iterator lastHashId;

    for (sregex_iterator i = firstHashId; i != lastHashId; i++) {
        smatch currentHashIdMatch = *i;
        // I get from my current match the first capture group (brackets) of my pattern (\d+) which only contains the numerical id in the hash id
        string currentNumericId = currentHashIdMatch.str(1);
        // I remove the old hash id and create a new markdown link using it and insert the new link in its place
        result.erase(currentHashIdMatch.position() + numberOfNewCharactersAdded, currentHashIdMatch.length());
        result.insert(currentHashIdMatch.position() + numberOfNewCharactersAdded, "[#" + currentNumericId + "](" + config.repoIssuesUrl + currentNumericId + ")");
        // Regex smatch.position() was assigned before we replaced hash ids with urls
        // So we must account for that by counting number of new characters we have added, "4" is for the characters "[]()"
        numberOfNewCharactersAdded += 4 + config.repoIssuesUrl.length() + currentNumericId.length();
    }

    return result;
};

/**
 * @brief Replaces all plain text commit SHAs (e.g., 219c2149) with links to these commits on GitHub
 * @param pullRequestBody The original body/description of the pull request to do the replacements on
 * @return Pull request body/description after performing the replacements
 */
string replaceCommitShasWithLinks(string pullRequestBody) {
    // Here I match any commit SHA that starts at the beginning of a line or with a space or "(" before it
    // and ends with either anything other than a number or a letter or the end of the pull request body
    // the "?=" is a regex lookahead which detects this pattern but doesn't include it in the match
    // I am using this specific regex pattern based on how GitHub markdown interprets commit SHAs
    regex commitShaPattern(R"((^|[ (])([0-9a-f]{6,40})(?=[^0-9a-zA-Z]|$))");

    string result = pullRequestBody;
    size_t numberOfNewCharactersAdded = 0;
    sregex_iterator firstSha(pullRequestBody.begin(), pullRequestBody.end(), commitShaPattern);
    sregex_iterator lastSha;

    for (sregex_iterator i = firstSha; i != lastSha; i++)
    {
        smatch currentShaMatch = *i;
        // Here I have the number "2" as a parameter in the below functions since I am interested in the second capture group
        // meaning the second "()" in the regex pattern which is the commit SHA itself
        string currentSha = currentShaMatch.str(2);
        result.erase(currentShaMatch.position(2) + numberOfNewCharactersAdded, currentShaMatch.length(2));
        result.insert(currentShaMatch.position(2) + numberOfNewCharactersAdded, "[" + currentSha.substr(0, 6) + "](" + config.repoCommitsUrl + currentSha + ")");
        numberOfNewCharactersAdded += 4 + config.repoCommitsUrl.length() + 6;
    }

    return result;
}

/**
 * @brief Remove extra new lines in retrieved PR description to make it look identical to the PR description on GitHub
 * @param pullRequestBody The original body/description of the retrieved PR
 * @return PR body/description after removing extra new lines
 */
string removeExtraNewLines(string pullRequestBody) {
    // New lines in the retrieved PR description are represented as "\r\n" and there is no problem with that
    // BUT after the script writes the retrieved PR description in the markdown file and I observed the contents of the markdown file
    // using a hexadecimal editor, I found out that for some reason an extra "\r" was added when the markdown was written, so new lines were "\r\r\n"
    // for some reason that created extra new lines, so when I tried removing this extra "\r" and I added 2 spaces before the new line "  \r\n"
    // (these 2 spaces in markdown specify that a new line should occur), it worked!
    regex pattern(R"(\r)");
    pullRequestBody = regex_replace(pullRequestBody, pattern, "  ");

    return pullRequestBody;
}

/**
 * @brief Makes the formatting of the retrieved PR body look like the PR on GitHub
 * @param pullRequestBody The original body/description of the retrieved PR
 * @return PR body/description after formatting it
 */
string formatPullRequestBody(string pullRequestBody) {
    pullRequestBody = replaceHashIdsWithLinks(pullRequestBody);
    pullRequestBody = replaceCommitShasWithLinks(pullRequestBody);
    pullRequestBody = removeExtraNewLines(pullRequestBody);

    return pullRequestBody;
}

/**
 * @brief Converts the given conventional commit title to a better markdown title that could be used in the release notes
 * Example: "fix: fixed bug X" gets converted to "### Fixed bug X"
 * @param conventionalCommitTitle The conventional commit title
 * @param matchResult CommitTypeMatchResult that this title got with it's conventional commit type (has subcategory or no)
 * @param markdownPrefix The markdown prefix (e.g. -, ##, ###, etc.) that should be added before the release note title
 * @return The improved markdown title
 */
string convertConventionalCommitTitleToReleaseNoteTitle(string conventionalCommitTitle, CommitTypeMatchResults matchResult, 
                                                        string markdownPrefix) {
    string subCategoryText = "";

    if (matchResult == CommitTypeMatchResults::MatchWithSubCategory) {
        size_t startPos = conventionalCommitTitle.find("(") + 1;
        // Adding the subcategory title
        subCategoryText = "(" + conventionalCommitTitle.substr(startPos, conventionalCommitTitle.find(")") - startPos) + " Related) ";
        // Capitalizing the first letter in the subcategory
        subCategoryText[1] = toupper(subCategoryText[1]);
    }

    string releaseNoteTitle = "";

    // Removing the commit type from the conventional commit title and capitalizing the first letter
    size_t colonPosition = conventionalCommitTitle.find(":");
    if (colonPosition != string::npos) {
        releaseNoteTitle = conventionalCommitTitle.substr(colonPosition + 2);
    }
    releaseNoteTitle[0] = toupper(releaseNoteTitle[0]);

    // Inserting the commit type subcategory (will be empty if there is no subcategory)
    releaseNoteTitle.insert(0, subCategoryText);

    // Adding the markdown prefix
    releaseNoteTitle = markdownPrefix + releaseNoteTitle + "\n";

    return releaseNoteTitle;
}
