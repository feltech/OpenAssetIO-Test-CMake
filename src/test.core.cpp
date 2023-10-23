// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 The Foundry Visionmongers Ltd

#include <openassetio/trait/TraitsData.hpp>

int main() {
  auto traits = openassetio::trait::TraitsData::make();
  traits->setTraitProperty("a", "b", openassetio::Str{"c"});
}
