# Battleship

## Build guide

### UNIX
- `git clone <battleship_url>`
- `cd Battteship`
- `git submodule update --init --recursive`
- `cmake -S . -B build`
- `cd build`
- `make`

### Windows
- `git clone <battleship_url>`
- `cd Battteship`
- `git submodule update --init --recursive`
- `cmake -S . -B build -G <present_visual_studio_version>`
- open `build\ALL_BUILD.vcxproj`
- build the solution
- copy the DLLs, built from dependencies of `Battleship`, to the `build` subdirectory with `battleship.exe`

## Contrib guide
The Gitflow shall be used.

1. create an issue, representing a change you wish to implement using one of these labels:
    * `feature` 
    * `improvement` 
    * `bugfix` 
2. create a branch off the `develop` branch, named using this scheme:
    * `feature` label -> `ft-<issue_number>-<short_description>` 
    * `improvement` label -> `imp-<issue_number>-<short_description>` 
    * `bugfix` label -> `bf-<issue_number>-<short_description>` 
3. create a PR with a short description of changes and evidence of the issue resolved, such as:
    * reference to an automatic test(s) to run
    * steps to manually verify the issue is resolved
    * screenshots or screencasts
4. await the repo admin's approval
