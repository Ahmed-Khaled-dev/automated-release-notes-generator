<h1 align="center">
  <br>
    <img src="https://upload.wikimedia.org/wikipedia/commons/1/10/Synfig_logo.svg" alt="Synfig" width="150">
    <img src="https://avatars.githubusercontent.com/u/18133?s=280&v=4" alt="Git" width="180" style="position:relative; bottom:17px;">
  <br>
  Synfig Automated Release Notes Generator
  <br>
</h1>



<h4 align="center">A C++ script, to demonstrate the ability to work on the <a href="https://www.synfig.org/" target="_blank">Synfig</a> 
<a href="https://summerofcode.withgoogle.com/" target="_blank">GSoC</a> 2024 
<a href="https://synfig-docs-dev.readthedocs.io/en/latest/gsoc/2024/ideas.html#projects-ideas" target="_blank">Automated release notes generator project</a>, the script generates markdown release notes from <a href="https://www.conventionalcommits.org/en/v1.0.0/" target="_blank">conventional git commits</a>.</h4>

<p align="center">
<a href="https://ahmed-khaled-dev.github.io/automated-release-notes-generator/files.html">
        <img src="https://img.shields.io/badge/Code Docs- Webpage-blue?style=for-the-badge&logo=none" >
</a>
</p>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#how-to-build">How To Build</a> •
  <a href="#notes">Notes</a>
</p>

<p align="center">
    <img src="https://i.imgur.com/Wsld5Xz.png" alt="Script Markdown Output">
</p>

## Key Features
- Generate markdown release notes from **normal commits and group them by type** (Bug fixes, New Features, Code Refactoring, etc.)
- Generate **2 modes** of markdown release notes from **merge commits** (Short/Full modes)
  - **Short mode**, release notes are made up from the **Pull request titles** of each merge commit
  - **Full mode**, release notes are made up from **Pull request title and Pull request body/description** of each merge commit
    
    <img src="https://i.imgur.com/Z8LRRgz.png" alt="Automated release notes generator">

## How To Use

> [!IMPORTANT]
> For any of the following, you'll need <a href="https://git-scm.com/" target="_blank">Git</a> installed on your computer.

<details>
  <summary>Linux</summary>
  
  1. Download the [AppImage](https://drive.google.com/file/d/1blyizMY7ejLM9J2-BILO_K0kKfU_UZsl/view?usp=sharing)
   
  2. Copy the AppImage into a directory that contains a git repo that uses conventional commits, If you don't have a repo that uses conventional commits, you can download <a href="https://drive.google.com/drive/folders/1RSMgekJLUEygaDpKpFud_DdF0AQTsKRx?usp=sharing" target="_blank">Synfig's .git folder</a> and test the script on it
   
  3. Make the AppImage executable
      ```
      $ chmod +x release_notes_generator.AppImage
      ```
  4. Run the AppImage from the command line using suitable parameters
      ``` 
      $ ./release_notes_generator.AppImage commit_type [release_notes_mode]
      ```
      1. Generate release notes from **normal commits**
          ``` 
          $ ./release_notes_generator.AppImage n
          ```
      2. Generate **short** release notes from **merge commits** (Internet connection required)
          ``` 
          $ ./release_notes_generator.AppImage m s
          ```
      3. Generate **full** release notes from **merge commits** (Internet connection required)
          ``` 
          $ ./release_notes_generator.AppImage m f
          ```
</details>
<details>
  <summary>Windows</summary>
  
  1. Download the <a href="https://drive.google.com/drive/folders/1ustJy8Znff_TrAfIbGwQJpSmiq4ULA_h?usp=sharing" target="_blank">executable and dlls</a>
   
  2. Copy the files into a directory that contains a git repo that uses conventional commits, If you don't have a repo that uses conventional commits, you can download <a href="https://drive.google.com/drive/folders/1RSMgekJLUEygaDpKpFud_DdF0AQTsKRx?usp=sharing" target="_blank">Synfig's .git folder</a> and test the script on it
   
  3. Run the executable from the command line using suitable parameters
      ``` 
      $ release_notes_generator commit_type [release_notes_mode]
      ```
      1. Generate release notes from **normal commits**
          ``` 
          $ release_notes_generator n
          ```
      2. Generate **short** release notes from **merge commits** (Internet connection required)
          ``` 
          $ release_notes_generator m s
          ```
      3. Generate **full** release notes from **merge commits** (Internet connection required)
          ```
          $ release_notes_generator m f
          ```
</details>

## How To Build

  ### 1. Clone the repo
  ```
  $ git clone https://github.com/Ahmed-Khaled-dev/automated-release-notes-generator.git
  ```

  ### 2. Install Dependencies
  The program has only 2 dependencies

  1. [libcurl](https://github.com/curl/curl)
    
      The easiest way to install libcurl is using any package manager ([chocolatey](https://github.com/chocolatey/choco) or [vcpkg](https://github.com/microsoft/vcpkg) for Windows and your distro's package manager in Linux)
    
  2. [nlohmann/json.hpp](https://github.com/nlohmann/json)
      
      Download the json.hpp file in the same directory that the script is in, on Windows open [json.hpp](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) and then download raw file, 
      on Linux run:

        ```
        $ wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
        ```
  
  ### 3. Execute the Makefile
  ```
  $ make
  ```

## Notes
- The current prototype displays a maximum of 3 release notes for each commit type section to prevent excessive markdown output in large repositories like Synfig's. Future iterations may include options to customize this limit via an external configuration file or through user input or based on the date of the last commit or git tags, etc.
- The current prototype demonstrates the display of three common commit types (fix, feat, and refactor). later on these can be modified very easily
