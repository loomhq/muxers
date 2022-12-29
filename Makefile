SHELL := /bin/bash

.PHONY: clean
clean:
	@-rm -rf .build/
	@-rm -rf build/
	@-rm -rf *.xcodeproj
	@-rm -rf .swiftpm

.PHONY: setup-brew
setup-brew:
	@brew bundle

.PHONY: setup-githooks
setup-githooks:
	git config --local include.path ./.gitconfig

.PHONY: setup
setup: setup-brew setup-githooks

.PHONY: build-c
build-c:
	cmake --preset default
	cmake --build --clean-first --preset default

.PHONY: test-c
test-c:
	cmake --preset default
	cmake --build --clean-first --preset default
	ctest --preset default

.PHONY: build-swift-macos
build-swift-macos:
	@swift --version
	@swift build

.PHONY: build-swift-ios
build-swift-ios:
	@swift --version
	xcodebuild -showsdks
	@swift build -v -Xswiftc "-sdk" -Xswiftc "`xcrun --sdk iphonesimulator --show-sdk-path`" -Xswiftc "-target" -Xswiftc "x86_64-apple-ios13.0-simulator"

.PHONY: format-clang
format-clang:
	@find src/tsmuxer/ -iname *.h -o -iname *.cpp | xargs clang-format -style=file -i

.PHONY: build
build: build-swift-macos build-swift-ios build-c

.PHONY: test
test: test-c

.PHONY: xcode
xcode:
	@open -a Xcode Package.swift
