# Wima

Wima (pronounced "WEE-muh") is a non-modal GUI framework built on top of
[GLFW](http://www.glfw.org/).

It can be seen as a "window manager" for user applications. In fact, that's
where its name came from: the author(s) wanted to use the prefix `WM_` for all
public functions, and "Wima" ended up being the nicest-sounding non-existent
word that allowed the prefix.

(Actually, the first name used was actually "Waimo," which was changed to not
clash with [Google's Waymo](https://waymo.com/).)

Wima is inspired by the windowing system used by [Blender](https://www.blender.org/).
While Blender is built on top of the C++ OpenGL library
[GHOST](http://www.letworyinteractive.com/blendercode/d5/d2e/GHOSTPage.html)
(written by the Blender developers), Wima uses GLFW, a third-party OpenGL
library.

Wima is developed by [Yzena](http://yzena.com/).

Wima is Free and Open Source Software (FOSS). It is licensed under a modified
[Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0.html). The
[modification](http://graphics.pixar.com/opensubdiv/docs/license.html) was first
used by [Pixar Animation Studios](https://www.pixar.com/) for use on the
[OpenSubdiv](http://graphics.pixar.com/opensubdiv/docs/intro.html) project.

## Getting Started

To get started, clone the repo, and then run the following commands in the root
directory:

```
git config --local include.path ../.gitconfig
git submodule update --init --recursive
```

## Dependencies

Wima depends on CMake, X11, Xinerama, Xcursor, Xi, and GLFW. On Ubuntu, they can
be installed with the following command:

```
sudo apt install libx11-dev libxinerama-dev libxcursor-dev libxi-dev libglfw3-dev cmake cmake-curses-gui
```

It also depends on [Yc](https://github.com/YzenaTech/Yc) and
[Dyna](https://github.com/YzenaTech/Dyna).

## Status

Wima is not even in Alpha stage yet; the first prototype has not yet been
completed. This software is nowhere near ready for use. However, it is being
developed alongside [Yera](https://github.com/YzenaTech/Yera).

## Language

Wima is written in pure ISO C11.

## Git Workflow

Wima uses the git workflow described
[here](http://endoflineblog.com/oneflow-a-git-branching-model-and-workflow).
Developers who want to contribute to Wima are encouraged to read that post
carefully.

For feature branches, it uses `rebase + merge --no-ff` (option 3). It also uses
a `develop`/`master` split. (Main development is on `develop`, and `master` just
points to the latest tagged release to make it easy for users to get the latest
release.)

## Commit Messages

Projects under Yzena, including Wima, use the commit message guidelines laid out
in [this blog post](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

## Semantic Versioning

Wima uses [semantic versioning](http://semver.org/).

## Contents Listing

Every folder contains a README file which lists the purposes for the files and
folders in that directory.

## Contents

Files:

	CMakeLists.txt  CMake is the build system used for Wima. This is the root
	                config file.
	LICENSE.md      A Markdown version of Pixar's modified Apache 2.0 license.
	NOTICE.md       The NOTICE file required by the Apache License.

Folders:

	docs        Contains all of the documentation for Wima (currently empty).
	include     Contains all of the public header files for Wima.
	src         All of Wima's source code.
	tests       Tests for Wima.

