# Wima

[TOC]

# Abstract {#wima-abstract}

This is the introduction to Wima. It includes a high-level overview about how to
use Wima in a project.

# Introduction {#wima-intro}

Wima is a non-modal GUI framework built on top of GLFW. It can be understood as
a "window manager" for user applications (called "clients" from here on out).

Because Wima is built on top of GLFW, a library that creates and manages windows
and contexts for OpenGL, Wima does all of its drawing with OpenGL.

You can learn more by either the [Overview](#overview), which gives a high-level
overview of how clients can use Wima, or [Concepts](#concepts), which gives a
detailed look at all of Wima's concepts and how they relate.

# Overview {#wima-overview}

Wima is distributed as a library. Clients link to it and use its API to create
and draw windows, as well as handle any events the user generates.

See [`tests/app.c`](../tests/app.c) for an example, but a typical example might
go as follows:

1.	A client must initialize Wima before doing anything else. That is done by
	using `wima_init()`.
2.	The client registers up to eight (and more than zero) of
	[regions](#concepts-types-regions) using `wima_region_register()`.
3.	The client uses those [regions](#concepts-types-regions) to register an
	[editor](#concepts-types-editors) using `wima_editor_register()`.
4.	The client repeats steps 3 and 4 as much as necessary to register all of its
	[editors](#concepts-types-editors).
5.	The client creates a [tree](#concepts-types-trees) using
	`wima_tree_create()`.
6.	The client recursively builds the tree using `wima_tree_addRootParent()`,
	`wima_tree_addRootEditor()`, `wima_tree_addLeftParent()`,
	`wima_tree_addLeftEditor()`. `wima_tree_addRightParent()`, and
	`wima_tree_addRightEditor()`.
7.	The client registers a [dialog](#concepts-types-dialogs) (using
	`wima_dialog_register()`) and/or a [workspace](#concepts-types-workspaces)
	(using `wima_workspace_register()`) from the [tree](#concepts-types-trees).
8.	If the client has more [dialogs](#concepts-types-dialogs) and/or
	[workspaces](#concepts-types-workspaces) to create, it resets the
	[tree](#concepts-types-trees) using `wima_tree_reset()` and repeats steps 4
	and 5. Otherwise, it frees the [tree](#concepts-types-trees) using
	`wima_tree_free()` and continues to step 7.
9.	The client creates a [window](#concepts-types-windows) using
	`wima_window_create()`.
10.	The client runs the event loop in `wima_main()`, which will return when the
	user has closed all [windows](#concepts-types-windows).
11.	The client exits by calling `wima_exit()`.

# Concepts {#wima-concepts}

Concepts are the ideas/things that make Wima. These are the things on top of
which Wima is built, as well as how the API is partitioned.

Wima has many concepts, and it's important to keep them and their relationships
to each other straight.

## Relationships {#wima-concepts-relationships}

Wima's concepts relate to each other in two ways:

*	One concept can be a [template](#wima-concepts-relationships-templates) for
	another.
*	One concept can [use](#wima-concepts-relationships-use) another.

### Templates {#wima-concepts-relationships-templates}

When a concept is a template for another concept (which is a concrete version of
the template), that means that the client builds the template concept first, and
then when a window is created, it uses that template to initialize the window
with the concrete version that the window can use.

### Use {#wima-concepts-relationships-use}

## Types

These are the concepts (concept types) in Wima.

### Callbacks {#wima-concepts-types-callbacks}

### Events {#wima-concepts-types-events}

### Images {#wima-concepts-types-images}

Images are textures that can be used to render parts of the user interface.

### Theme {#wima-concepts-types-theme}

### Render {#wima-concepts-types-render}

### Style {#wima-concepts-types-style}

### Paths {#wima-concepts-types-paths}

### Text {#wima-concepts-types-text}

### Colors {#wima-concepts-types-colors}

### Paints {#wima-concepts-types-paints}

### Icons {#wima-concepts-types-icons}

### User Interface {#wima-concepts-types-user_interface}

### Properties {#wima-concepts-types-properties}

### Widgets {#wima-concepts-types-widgets}

### Layouts {#wima-concepts-types-layouts}

### Trees {#wima-concepts-types-trees}

### Workspaces {#wima-concepts-types-workspaces}

***TODO***: Add a picture to show workspace tabs.

### Dialogs {#wima-concepts-types-dialogs}

### Sections {#wima-concepts-types-sections}

### Regions {#wima-concepts-types-regions}

### Editors {#wima-concepts-types-editors}

Editors are templates for [areas](#wima-concepts-types-areas).

### Areas {#wima-concepts-types-areas}

### Menus {#wima-concepts-types-menus}

### Overlays {#wima-concepts-types-overlays}

### Windows {#wima-concepts-types-windows}

### Cursors {#wima-concepts-types-cursors}

### Monitors {#wima-concepts-types-monitors}
