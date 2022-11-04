// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 The Foundry Visionmongers Ltd
#include <stdio.h>

// Include all headers to test they are where we expect and can be
// compiled.
#include <openassetio/c/InfoDictionary.h>
#include <openassetio/c/StringView.h>
#include <openassetio/c/errors.h>
#include <openassetio/c/hostApi/Manager.h>
#include <openassetio/c/managerApi/CManagerInterface.h>
#include <openassetio/c/managerApi/HostSession.h>
#include <openassetio/c/managerApi/ManagerInterface.h>
#include <openassetio/c/namespace.h>

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
