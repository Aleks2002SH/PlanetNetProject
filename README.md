# PlanetNetProject

Interactive Qt/QML application that models an interplanetary industrial and transport network with a 3D globe interface, an embedded SQLite database, and tools for exploring planetary resources and bases.

## Overview

PlanetNetProject was developed at Lomonosov Moscow State University (Faculty of Space Research) by a student project group as a technical prototype for an "interplanetary transport–industrial network".

The application is built with **Qt Creator** using **C++** and **QML/QtQuick3D** and includes:

- a visual interface with 3D views of several planets and moons (Moon, Mars, Phobos, Ceres, Titan);
- an embedded **SQLite** database storing information about celestial bodies, locations, bases, minerals, chemical elements, materials, and products;
- interactive tools for planet selection, zoom and rotation, adding new bases at specified coordinates, and inspecting resource data via context menus.

## Features

- **Planet selection and 3D visualization**  
  Choose a planet or moon in the Solar System and view it as a 3D globe rendered with QtQuick3D.

- **Surface maps and zoom**  
  Display raster maps/textures on the planetary surface, zoom in and out, and rotate the globe using mouse controls.

- **Base creation and coordinate input**  
  Add new industrial bases on the surface by specifying coordinates; bases are visualized with simple 3D primitives tied to their positions.

- **Context menus and info panels**  
  Open context menus and information panels to view details from the database: location descriptions, minerals, chemical elements, and materials/products associated with each base or area.

- **SQLite-backed data model**  
  Use an embedded SQLite database for persistent storage of planets, locations, areas, minerals, elements, and industrial objects. Data can be read and updated directly from the application.

- **Basic physical modeling**  
  Include functions for computing gravitational influence and for converting geographic coordinates (latitude/longitude) to 3D positions on the planet surface, taking into account radius, rotation, and planet position.

## Code structure

The main components of the project are:

- `Main.qml` — QML-based 3D visualizer of planets and moons with interactive camera, planet selection UI, marker placement, and info panel.
- `Functions.cpp` — helper functions, including gravitational influence calculations and other utilities.
- `Net_bases.cpp` — C++ class responsible for working with bases and locations: reading data from SQLite, parsing coordinates, converting latitude/longitude to 3D positions, and providing information about each location.[file:138]
- `Net_new_elements.cpp` — C++ class for handling geographic areas on celestial bodies: loading area boundaries from the database, checking which area a point belongs to, and returning minerals and elements for that area.[file:138]
- `Create_and_fill_interplanetary_database` (notebook/script) — tools for creating and populating the interplanetary SQLite database with test data.

## Technologies

- Qt Creator
- C++
- QML / QtQuick3D
- SQLite

## Usage

This project is primarily an educational and research prototype. It demonstrates how to:

- combine QtQuick3D 3D visualization with Qt Quick UI elements;
- integrate an SQLite database into a Qt/QML application;
- model planetary bases and resources in a simple interplanetary network.

Clone the repository and open the project in Qt Creator to build and run the application on your system. Make sure you have Qt with QtQuick3D and Qt SQL modules installed.
