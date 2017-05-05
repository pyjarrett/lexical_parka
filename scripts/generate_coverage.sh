#!/usr/bin/env bash
pushd ../build
lcov --directory . --capture --output-file app.info
genhtml -o ../doc/coverage_reports/ app.info
firefox ../doc/coverage_reports/index.html &
popd
