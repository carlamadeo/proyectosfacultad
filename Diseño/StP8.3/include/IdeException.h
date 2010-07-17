#ifndef IdeException_H
#define IdeException_H

#include "IdeToolsApi.h"
#include "IdeString.h"

// Please throw exception objects themselves, don't throw pointers to them.

// Base class for all StP exceptions, not intended to be thrown directly
class IDE_TOOLS_CLASS_API IdeException
{
protected:
    IdeException() {}

public:
    virtual ~IdeException();

    // The getErrorMessage function returns pointer to an error message
    // explaining exception. If no message is available the function
    // returns an empty string. By convention this means that the error
    // has already been reported to the user.
    //
    // The returned pointer is valid only until the exception object changes
    virtual const char* getErrorMessage();

    // Displays error message to the user. Returns true if successful and
    // false if no error message is available
    virtual bool reportError();
};

// This exception is thrown when "new" or "malloc" fails
class IDE_TOOLS_CLASS_API IdeMemoryException : public IdeException
{
public:
    IdeMemoryException() {}
    virtual ~IdeMemoryException();

    // The returned pointer is valid only until the exception object changes
    virtual const char* getErrorMessage();
};

// This exception is thrown when the program cannot allocate
// some system resource.
class IDE_TOOLS_CLASS_API IdeResourceException : public IdeException
{
public:
    IdeResourceException(const char* aMessage = 0);
    virtual ~IdeResourceException();

    // The returned pointer is valid only until the exception object changes
    virtual const char* getErrorMessage();

protected:
    IdeString mMessage;
};

class IDE_TOOLS_CLASS_API IdeFileException;

// File I/O exception
class IDE_TOOLS_CLASS_API IdeFileException : public IdeException
{
public:
    enum CauseEnum
    {
        Generic,            // Unspecified error
        EndOfFile,          // End of file
        DiskFull,           // Disk full
        DirectoryFull,      // Directory full
        FileNotFound,       // File or directory not found
        BadPath,            // Bad file or directory name
        TooManyOpenFiles,   // Too many open files
        AccessDenied,       // Access denied
        SharingViolation,   // Access failed due to sharing violation
        LockViolation,      // Access failed because required part of the file is locked
        InvalidFileHandle,  // Invalid file handle
        AttemptToRemoveCurrentDir,   // An attempt to remove current directory
        BadSeek,            // Seek operation failed
        HardwareError,      // Hardware I/O error
        InvalidFileFormat   // Invalid file format
    };

    IdeFileException(CauseEnum aCause = IdeFileException::Generic,
                    const char* aFileName = 0);
    virtual ~IdeFileException();

    // The returned pointer is valid only until the exception object changes
    virtual const char* getErrorMessage();

    // Converts errno code to CauseEnum
    static CauseEnum errnoToCause(int anErrno);

    // Throws file exception based on errno
    static void throwErrno(int anErrno, const char* aFileName = 0); // throw (IdeFileException)

    CauseEnum mCause;           // Cause of exception
    IdeString mFileName;

protected:
    IdeString mMessage;
};

// A IdeReportedException is thrown to stop an end-user operation.
// Use IdeReportedException when you want to use the throw/catch exception
// mechanism for application-specific exceptions. "User" in the class name
// can be interpreted as "my user did something exceptional that I need to handle."
// A IdeReportedException is usually thrown after calling a notification function
// of your choice to notify the user that an operation has failed.
class IDE_TOOLS_CLASS_API IdeReportedException : public IdeException
{
public:
    IdeReportedException();
    virtual ~IdeReportedException();
};

// This exception is thrown when user cancellation occurs
class IDE_TOOLS_CLASS_API IdeCancellationException : public IdeException
{
public:
    IdeCancellationException() {}
    virtual ~IdeCancellationException() {}

    // The returned pointer is valid only until the exception object changes
    virtual const char* getErrorMessage() { return "Cancelled"; }
};

#endif // IdeException_H
