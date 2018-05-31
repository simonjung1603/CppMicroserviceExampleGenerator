#pragma once

int isTemplatePathValid(std::experimental::filesystem::v1::path &templatePath, char ** argv, bool &retflag);

void removeTrailingSeparator(std::experimental::filesystem::v1::path &p);
