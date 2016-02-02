/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef tutorial_FileIO_h
#define tutorial_FileIO_h

#include <parc/algol/parc_Buffer.h>

/**
 * Given a fileName and chunk number, retrieve that chunk from the specified file. The
 * contents of the chunk are returned in a PARCBuffer that must eventually be released
 * via a call to parcBuffer_Release(&buf). The chunkNumber is 0-based.
 *
 * @param [in] fileName A pointer to a string containing the name of the file to read from.
 * @param [in] chunkSize The maximum number of bytes to be returned in each chunk.
 * @param [in] chunkNumber The 0-based number of chunk to return from the file.
 *
 * @return A newly created PARCBuffer containing the contents of the specified chunk.
 */
PARCBuffer *tutorialFileIO_GetFileChunk(const char *fileName, size_t chunkSize, uint64_t chunkNumber);

/**
 * Given a PARCBuffer, append its contents to the file specified by the given fileName.
 *
 * @param [in] fileName A pointer to a string containing the name of the file to write to.
 * @param [in] chunk A pointer to a PARCBuffer containing the bytes to append to the file.
 *
 * @return The number of bytes written to the file.
 */
size_t tutorialFileIO_AppendFileChunk(const char *fileName, const PARCBuffer *chunk);

/**
 * Check if a file exists and is readable.
 * Return true if it does, false otherwise.
 *
 * @param [in] fileName A pointer to a string containing the name of the file to test.
 *
 * @return true If the file exists and is readable.
 * @return false If the file doesn't exist or is not readable.
 */
bool tutorialFileIO_IsFileAvailable(const char *fileName);

/**
 * Return the size, in bytes, of the specified file.
 *
 * @param [in] fileName A pointer to a string containing the name of file from which to get the size.
 *
 * @return The size of the file, in bytes.
 */
size_t tutorialFileIO_GetFileSize(const char *fileName);

/**
 * Delete the file specified.
 *
 * @param [in] fileName A pointer to a string containing the name of the file to delete.
 *
 * @return true If the file was successfully deleted.
 * @return false If the file was not deleted.
 */
bool tutorialFileIO_DeleteFile(const char *fileName);

/**
 * Return a PARCBuffer containing a string representing the list of files and their sizes in the directory
 * specified by 'dirName'. File names and sizes in the returned string are seperated by newlines. This
 * function does not recurse into subdirectories.
 *
 * The returned PARCBuffer must eventually be released via a call to parcBuffer_Release().
 *
 * @param dirName A pointer to a string containing the name of the directory to inspect.
 */
PARCBuffer *tutorialFileIO_CreateDirectoryListing(const char *dirName);
#endif // tutorial_FileIO_h
