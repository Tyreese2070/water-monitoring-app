# COMP2811 UI Final Project Coursework

## Overview
This application is designed to monitor and analyse water quality data, with a focus on pollutant tracking and compliance with safety standards. It was developed using **C++/Qt6** with a modular and user-friendly interface that adapts to changes in locale for basic internationalisation.

## Team Members
- Reuben Edels
- Charlie Jheeta
- Noah Davis
- Tyreese Green
- Smriti Umesh

## Getting Started

### Prerequisites
- **C++17** or higher
- **Qt6** libraries, including:
    - **QtWidgets**
    - **QtCharts**
    - **QtWebView** 

### Installation and Setup
1. **Extract the Starter Code**: 
 - Unzip the provided starter code package to a working directory.
2. **Build the Project**:
    - Enter the `startercode` directory
    - Create a `build` directory and run `cmake`:
      ```bash
      mkdir build && cd build
      cmake ..
      ```
    - Compile the project:
      ```bash
      make
      ```
    - Run the application:
      ```bash
      ./pollutanttool
      ```

## Project Overview
### Interface Contols
- **Import Data Button**: Opens a file select interface, allowing users to choose a dataset.
- **Filter tools**: Filters the dataset across all pages by site and dataset.
- **User guide**: Walks through the application, opens on first start-up.
- **About**: Provides a brief description of the project and Qt6.
- **References**: Provides links to any material used to help create the application.

### Pages
- **Dashboard**: Provides an overview of the dataset, showing a breakdown of each pollutant category.
- **Category Pages**: More detailed view on specific pollutant categories, showing a breakdown of data thrugh charts, as well as providing information on the pollutant category.
    - **Common Pollutants**: Displays a bar chart and pie chart showing common pollutant levels across various sites.
    - **Persistent Organic Pollutants**: Displays a time-series chart and pie chart showing levels of compliance.
    - **Litter and Sewage**: Displays a bar chart and pie chart showing litter and sewage levels across various sites.
    - **Fluorinated Pollutants**: Displays a time-series chart and pie chart showing levels of compliance.
- **Compliance Statistics**: Displays compliant, non-compliant and cautious sites for given pollutants or categories.
- **Table Page**: Displays pollutant data in a table format, allowing us to browse relevant fields. Utilises the model/view architecture from the starter code.

## Key Features
- **Internationalisation**: The app supports German translations when locale settings are changed.
   - *for testing this functionality, one translation has been provided in the filters on the left-hand side of the application*
- **Modular UI**: Using a "card" approach, each pollutant type is grouped logically within the interface.
- **Tooltips**: Hovering over charts displays additional context, improving usability.

## Design Choices
- **Model/View Architecture**: Chosen for flexibility in data handling and to separate data presentation from underlying storage.
- **Data Visualisation**: Includes basic charts and compliance indicators to ensure intuitive data interpretation.
- **"Card" Layout**: The UI is modularised into "cards,” each focused on a pollutant group or unique data display.

## Included Files
- **Source Files**: Core source files such as `main.cpp`, `dataset.cpp`, `tablemodel.cpp`, etc.
- **Icon Files**: Images displayed on the User Interface.
- **Dictionary Files**: Provides data on specific pollutants, i.e. their category and legal limits.
- **Final Report**: PDF document detailing the development and design of the application.
- **Video Files**: Videos documenting iterations of the application and showcasing the finished application.
- **Ethics Documentation**: Participant Information Sheet and blank consent form.

## Future Development
- **Location Search Bar**: Allows for filtering using a search bar instead of a dropdown menu.
- **Loading Bar**: An indicator displayed whilst a dataset is being loaded into the application.
- **Geographical Hotspots Map**: Represents locations of pollutants on an interactive map.

## References
- https://echa.europa.eu/
- https://environment.data.go.uk/
- https://theriverstrust.org/
