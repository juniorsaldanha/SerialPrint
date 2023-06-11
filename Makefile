# Makefile - Version information for SerialPrint application

# Define the path to the library
PROJECT_PATH := ./
# Define the path to the Version.h file
VERSION_PATH := $(PROJECT_PATH)src/SerialPrintVersion.hpp

# Extract the version variables from the Version.h file
PROJECT_NAME := $(shell cat $(VERSION_PATH) | sed -En 's/\#define FIRMWARE_PROJECT_NAME "(.*)"/\1/p')
MAJOR_VERSION := $(shell cat $(VERSION_PATH) | sed -En 's/\#define FIRMWARE_VERSION_MAJOR (.*)/\1/p' | cut -d' ' -f2)
MINOR_VERSION := $(shell cat $(VERSION_PATH) | sed -En 's/\#define FIRMWARE_VERSION_MINOR (.*)/\1/p' | cut -d' ' -f2)
PATCH_VERSION := $(shell cat $(VERSION_PATH) | sed -En 's/\#define FIRMWARE_VERSION_PATCH (.*)/\1/p' | cut -d' ' -f2)

# Concatenate the version variables into a single variable
PROJECT_VERSION := $(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)

# Release folder name
RELEASE_FOLDER := release_$(PROJECT_VERSION)

.PHONY get-version:
	@echo "Project: $(PROJECT_NAME)"
	@echo "Version: $(PROJECT_VERSION)"

.PHONY: --wrape-release

--wrape-release:
	@echo "Wrape $(PROJECT_NAME) $(PROJECT_VERSION)"

	@mkdir $(RELEASE_FOLDER)

	@echo "Copying files to $(RELEASE_FOLDER)"
	@cp -r $(PROJECT_PATH)examples $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)src $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)library.properties $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)README.md $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)CHANGELOG.md $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)LICENSE $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)Makefile $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH)Doxyfile $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH).clang-format $(RELEASE_FOLDER)
	@cp -r $(PROJECT_PATH).gitignore $(RELEASE_FOLDER)

	@echo "Zipping $(PROJECT_NAME) $(PROJECT_VERSION)"
	@zip -r $(RELEASE_FOLDER).zip $(RELEASE_FOLDER)

# Create the changelog
generate-changelog:
	@echo "Generating changelog for $(PROJECT_NAME) $(PROJECT_VERSION)"
	@auto-changelog --hide-credit --release-summary

# Create the release, check if the version is already released
create-release:
	@echo "Releasing $(PROJECT_NAME) $(PROJECT_VERSION)"
	@$(--wrape-release)
	@sed -i '' 's/version=.*/version=$(PROJECT_VERSION)/g' library.properties
	@git tag -l | grep -q $(PROJECT_VERSION) && echo "Version $(PROJECT_VERSION) already released" && exit 1 || echo "Releasing version $(PROJECT_VERSION)"
	@git tag $(PROJECT_VERSION)
	# @git push --tags