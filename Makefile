SHELL := /bin/bash

.PHONY: clean
clean:
	@-rm -rf .build/
	@bazel clean
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
	@bazel build //tsmuxer:tsmuxer 2>&1

.PHONY: test-c
test-c:
	@bazel test --test_output=streamed --test_timeout=700 //tsmuxer/tests:tsmuxer_tests

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
