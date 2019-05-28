# Optimization of Vehicular Communication

This repository contains multiple subprojects, which are related to the topics *machine learning-based data rate prediction in vehicular networks* and *data-driven optimization of cognitive networking*. All projects are based on Qt-C++.

- The data acquistion is performed with the Android-based **Client** application, which measures network quality information and transmits data to a remote server.
- Different end-to-end performance indicators are obtained in a cooperative manner at the  **Server** side
- *Machine learning* is performed with **WEKA** as an external tool. In order to allow automated model training and generation of online prediction models, WEKA is controlled with an interface application.
- The acquired data and the trained models are exploited by the **DDNS** in order to analyze and optimize new cognitive communication methods.

Further setup instructions can be found in the different subprojects themselves.
