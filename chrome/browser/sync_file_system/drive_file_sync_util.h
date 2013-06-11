// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_FILE_SYNC_UTIL_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_FILE_SYNC_UTIL_H_

#include "chrome/browser/google_apis/gdata_errorcode.h"
#include "webkit/browser/fileapi/syncable/sync_status_code.h"

namespace sync_file_system {

// Translates GDataErrorCode to SyncStatusCode.
SyncStatusCode GDataErrorCodeToSyncStatusCode(
    google_apis::GDataErrorCode error);

// Enables or disabled Drive API in Sync FileSystem API.
// TODO(nhiroki): This method should go away when we completely migrate to
// DriveAPI. (http://crbug.com/234557)
void SetEnableDriveAPI(bool flag);

// Returns true if we enable Drive API in Sync FileSystem API.
// It is disabled by default but can be overridden by a command-line switch
// (--enable-drive-api-for-syncfs) or by calling
// SetEnableDriveAPI().
// TODO(nhiroki): This method should go away when we completely migrate to
// DriveAPI. (http://crbug.com/234557)
bool IsDriveAPIEnabled();

}  // namespace sync_file_system

#endif  // CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_FILE_SYNC_UTIL_H_