# Wima

This is the introduction to Wima.

## About Wima

Wima is a minimal, non-modal GUI framework built on top of GLFW. It can be
understood as a "window manager" for user applications (called "clients" from
here on out).

Because Wima is built on top of GLFW, a library that creates and manages windows
and contexts for OpenGL, Wima does all of its drawing with OpenGL.

You can learn more by either the [Overview](#overview), which gives a high-level
overview of how clients can use Wima, or [Concepts](#concepts), which gives a
detailed look at all of Wima's concepts and how they relate.

<a name="overview"></a>
## Overview

Wima is distributed as a library. Clients link to it and use its API to create
and draw windows, as well as handle any events the user generates.

See [`tests/app.c`](../tests/app.c) for an example, but a typical example might
go as follows:

1.	A client must initialize Wima before doing anything else. That is done by
	using `wima_init()`.
2.	The client registers any number (but more than zero) of
	[regions](#concepts-types-regions) using `wima_region_register()`.
3.	The client registers any number (but more than zero) of
	[editors](#concepts-types-editors) using `wima_editor_register()`.
4.	The client creates a [tree](#concepts-types-trees) using
	`wima_tree_create()`.
5.	The client recursively builds the tree using `wima_tree_addRootParent()`,
	`wima_tree_addRootEditor()`, `wima_tree_addLeftParent()`,
	`wima_tree_addLeftEditor()`. `wima_tree_addRightParent()`, and
	`wima_tree_addRightEditor()`.
6.	The client registers a [dialog](#concepts-types-dialogs) (using
	`wima_dialog_register()`) and/or a [workspace](#concepts-types-workspaces)
	(using `wima_workspace_register()`) from the [tree](#concepts-types-trees).
7.	If the client has more [dialogs](#concepts-types-dialogs) and/or
	[workspaces](#concepts-types-workspaces) to create, it resets the
	[tree](#concepts-types-trees) using `wima_tree_reset()` and repeats steps 4
	and 5. Otherwise, it frees the [tree](#concepts-types-trees) using
	`wima_tree_free()` and continues to step 7.
8.	The client creates a [window](#concepts-types-windows) using
	`wima_window_create()`.
9.	The client runs the event loop in `wima_main()`, which will return when the
	user has closed all [windows](#concepts-types-windows).
10.	The client exits by calling `wima_exit()`.

<a name="concepts"></a>
## Concepts

Concepts are the ideas/things that make Wima. These are the things on top of
which Wima is built, as well as how the API is partitioned.

Wima has many concepts, and it's important to keep them and their relationships
to each other straight.

<a name="concepts-relationships"></a>
### Relationships

Wima's concepts relate to each other in two ways:

*	One concept can be a [template](#concepts-templates) for another.
*	One concept can [use](#concepts-use) another.

<a name="concepts-templates"></a>
#### Templates

When a concept is a template for another concept (which is a concrete version of
the template), that means that the client builds the template concept first, and
then when a window is created, it uses that template to initialize the window
with the concrete version that the window can use.

<a name="concepts-use"></a>
#### Use

<a name="concepts-types"></a>
### Types

These are the concepts (concept types) in Wima.

<a name="concepts-types-callbacks"></a>
#### Callbacks

<a name="concepts-types-events"></a>
#### Events

<a name="concepts-types-images"></a>
#### Images

Images are textures that can be used to render parts of the user interface.

<a name="concepts-types-theme"></a>
#### Theme

<a name="concepts-types-render"></a>
#### Render

<a name="concepts-types-style"></a>
#### Style

<a name="concepts-types-paths"></a>
#### Paths

<a name="concepts-types-text"></a>
#### Text

<a name="concepts-types-colors"></a>
#### Colors

<a name="concepts-types-paints"></a>
#### Paints

<a name="concepts-types-icons"></a>
#### Icons

<a name="concepts-types-user_interface"></a>
#### User Interface

<a name="concepts-types-properties"></a>
#### Properties

<a name="concepts-types-widgets"></a>
#### Widgets

<a name="concepts-types-layouts"></a>
#### Layouts

<a name="concepts-types-trees"></a>
#### Trees

<a name="concepts-types-workspaces"></a>
#### Workspaces

***TODO***: Add a picture to show workspace tabs.

<a name="concepts-types-dialogs"></a>
#### Dialogs

<a name="concepts-types-sections"></a>
#### Sections

<a name="concepts-types-regions"></a>
#### Regions

<a name="concepts-types-editors"></a>
#### Editors

Editors are templates for [areas](#areas).

<a name="concepts-types-areas"></a>
#### Areas

<a name="concepts-types-menus"></a>
#### Menus

<a name="concepts-types-windows"></a>
#### Windows

<a name="concepts-types-cursors"></a>
#### Cursors

<a name="concepts-types-monitors"></a>
#### Monitors
