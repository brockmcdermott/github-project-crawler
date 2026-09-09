# GitHub Project Crawler

## Project Topic

GitHub Project Crawler: a C++ command-line application for exploring and collecting information about GitHub projects.

## Team

| Role | Full Name | A-Number | USU Email |
| --- | --- | --- | --- |
| Team Leader | Brock McDermott | A02370118 | a02370118@usu.edu |
| Jameson Irwin | A02309411 | a02309411@aggies.usu.edu | 

## Build Instructions

This project uses C++20, Clang/Clang++, CMake, Git, and command-line build tools.

From the root of the repository, run:

```bash
./build.sh
```

The script configures the project with CMake and builds it into the `build/` directory.

## Run Instructions

After building, run the executable from the repository root:

```bash
./build/github_project_crawler
```

## Clean Checkout Notes

A clean checkout should build successfully after CMake and Clang/Clang++ are installed and available on `PATH`.
