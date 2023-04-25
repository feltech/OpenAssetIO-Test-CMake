// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 The Foundry Visionmongers Ltd

#include <openassetio/TraitsData.hpp>

int main() {
  auto traits = openassetio::TraitsData::make();
  traits->setTraitProperty("a", "b", openassetio::Str{"c"});
}
