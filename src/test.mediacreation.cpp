// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 The Foundry Visionmongers Ltd

// Include all headers to test they are where we expect and can be
// compiled.
#include <openassetio/TraitsData.hpp>

#include <openassetio_mediacreation/openassetio_mediacreation.hpp>

int main() {
  auto trait = openassetio_mediacreation::traits::managementPolicy::ManagedTrait(
      openassetio::TraitsData::make());

  trait.imbue();
  return 0;
}
