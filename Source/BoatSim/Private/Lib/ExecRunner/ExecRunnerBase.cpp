// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/ExecRunner/ExecRunnerBase.h"
#include <windows.h>
#include <tlhelp32.h>

CExecRunnerBase::CExecRunnerBase()
{
}

CExecRunnerBase::~CExecRunnerBase()
{
}
bool CExecRunnerBase::ResumeProcess(uint32 processID) 
{
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE)
        return false;

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hThreadSnap, &te32)) {
        CloseHandle(hThreadSnap);
        return false;
    }

    // Loop through all threads in the snapshot.
    do {
        if (te32.th32OwnerProcessID == processID) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
            if (hThread == NULL)
                continue;

            ResumeThread(hThread);
            CloseHandle(hThread);
        }
    } while (Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
    return true;
}
bool CExecRunnerBase::RunExecutable(const FString& ExecutableRelativePath, const FString& Arguments)
{
#if PLATFORM_WINDOWS
    FString BaseDir = FPaths::ProjectDir(); // Gets the project directory
    FString ExecutableFullPath = FPaths::Combine(*BaseDir, *ExecutableRelativePath); // Combines the base directory with the relative path

    // Ensure the executable path exists
    if (!FPaths::FileExists(ExecutableFullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Executable does not exist at path: %s"), *ExecutableFullPath);
        return false;
    }
    uint32 outprocid;
    //FString outstd, outstderr;
    //bool retb = FPlatformProcess::ExecProcess(*ExecutableFullPath, *Arguments, &ret,&outstd,&outstderr);
    FProcHandle ProcHandle = FPlatformProcess::CreateProc(*FString("C:\\Users\\Pc\\Documents\\Unreal Projects\\BoatSim\\Sonar\\SonarSim\\x64\\Release\\SonarSimProj.exe")/* *ExecutableFullPath */, *Arguments, true, false, false, &outprocid, 10, nullptr, nullptr);

    if (!ProcHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start the external executable."));
        return false;
    }
    ResumeProcess(outprocid);
    UE_LOG(LogTemp, Log, TEXT("External executable started successfully."));
    return true;
#endif

    return false;
}


void CExecRunnerBase::ExecuteWindowsCommand(const FString& Command)
{
    // Create a process handle
    FProcHandle ProcHandle = FPlatformProcess::CreateProc(*Command, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);

    if (ProcHandle.IsValid())
    {
        // Optionally wait for the process to finish
        FPlatformProcess::WaitForProc(ProcHandle);

        // Close the process handle
        FPlatformProcess::CloseProc(ProcHandle);
    }
    else
    {
        // Handle the error if the process did not start
        UE_LOG(LogTemp, Warning, TEXT("Failed to start process with command: %s"), *Command);
    }
}
