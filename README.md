Wima
====

Wima (pronounced "WEE-muh") is a minimal, non-modal GUI framework built on top
of [GLFW](http://www.glfw.org/).

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

Wima is developed by [Project LFyre](http://lfyre.org/).

Wima is Free and Open Source Software (FOSS). It is licensed under a modified
[Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0.html). The
[modification](http://graphics.pixar.com/opensubdiv/docs/license.html) was first
used by [Pixar Animation Studios](https://www.pixar.com/) for use on the
[OpenSubdiv](http://graphics.pixar.com/opensubdiv/docs/intro.html) project.

Status
------

Wima is not even in Alpha stage yet; the first prototype has not yet been
completed. This software is nowhere near ready for use. However, it is being
developed alongside [Yera](https://github.com/ProjectLFyre/Yera).

Language
--------

Wima is written in pure ANSI C11.

Git Workflow
------------

Wima uses the git workflow described in
[this post](http://endoflineblog.com/oneflow-a-git-branching-model-and-workflow).
Developers who want to contribute to Wima are encouraged to read that post
carefully.

For feature branches, it uses "`rebase` + `merge --no--ff`" (option 3). It also
uses the `develop`/`master` split. (Main development is on `develop`, and
`master` just points to the lastest tagged release to make it easy for users to
get the latest release.

Wima includes scripts and a `.gitconfig` that helps manage the workflow. New
contributors should familiarize themselves with them.

Commit Messages
---------------

Projects under Project LFyre, including Wima, use the commit message guidelines
laid out in [this blog post](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

Semantic Versioning
-------------------

Wima uses [semantic versioning](http://semver.org/).

Contents Listing
----------------

Every folder contains a README file which lists the purposes for the files and
folders in that directory.

Contents
--------

Files:

	CMakeLists.txt	CMake is the build system used for Wima. This is the root
					config file.
	LICENSE.md		A Markdown version of Pixar's modified Apache 2.0 license.
	NOTICE.md		The NOTICE file required by the Apache License.

Folders:

	docs		Contains all of the documentation for Wima (currently empty).
	include		Contains all of the header files for Wima.
	src			All of Wima's source code.
	tests		Tests for Wima.

