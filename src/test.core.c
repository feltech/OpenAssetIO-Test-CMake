// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 The Foundry Visionmongers Ltd
#include <stdio.h>

#include <openassetio/c/InfoDictionary.h>
#include <openassetio/c/StringView.h>
#include <openassetio/c/errors.h>

#define kStringBufferCapacity 500

int main() {
  char errorMessageBuffer[kStringBufferCapacity];
  oa_StringView errorMessage = {kStringBufferCapacity, errorMessageBuffer, 0};

  oa_InfoDictionary_h infoDictionary;
  if (oa_InfoDictionary_ctor(&errorMessage, &infoDictionary) != oa_ErrorCode_kOK) {
    // No guarantee that string is null-terminated, so can't use `puts`
    // etc.
    fwrite(errorMessage.data, sizeof(char), errorMessage.size, stdout);
    return 1;
  }

  oa_InfoDictionary_dtor(infoDictionary);
}
