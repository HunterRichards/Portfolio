# CS-360 Warehouse Inventory Management App

<br><b>Briefly summarize the requirements and goals of the app you developed. What user needs was this app designed to address?</b></br>

This app was developed with the goal of streamlining inventory management in a warehouse setting. It addresses the need for efficient tracking and updating of inventory items, facilitating ease of access and manipulation of data for warehouse staff and managers. The core requirements included a database to store inventory items and user credentials, a login screen, a grid display of inventory items, and features for adding, removing, and updating item quantities. Additionally, it included a notification mechanism for items reaching zero quantity.

<br><b>What screens and features were necessary to support user needs and produce a user-centered UI for the app? How did your UI designs keep users in mind? Why were your designs successful?</b></br>

The app features several key screens: a login screen, an inventory display screen, and screens for adding or updating inventory items. The UI design is user-centered, focusing on simplicity and ease of navigation. Large, clear buttons and readable text fields were used to enhance usability. The inventory screen, with its grid layout, allows users to quickly view and assess inventory status. Success in design was achieved by prioritizing intuitive navigation and minimizing user input errors through clear, interactive elements.

<br><b>How did you approach the process of coding your app? What techniques or strategies did you use? How could those be applied in the future?</b></br>

The coding process involved a methodical and modular approach. Emphasis was placed on writing clear, maintainable code with comments for ease of understanding and future updates. Key strategies included:
- Utilizing Android’s RecyclerView for efficient display and management of inventory items.
- Implementing SQLiteOpenHelper for robust database interactions.
- Applying MVC (Model-View-Controller) principles to separate concerns and enhance code organization.

<br><b>How did you test to ensure your code was functional? Why is this process important and what did it reveal?</b></br>

Testing was conducted at multiple stages: unit testing for individual components, integration testing for database interactions, and UI testing for ensuring the user experience was as intended. Automated tests were written to check the functionality of critical aspects like database operations. Manual testing was also conducted to simulate real-user interaction scenarios. This process is crucial for identifying and rectifying bugs early, ensuring functionality aligns with user needs, and maintaining code quality. Testing revealed areas needing optimization and helped ensure reliability and user satisfaction.

<br><b>Considering the full app design and development process, from initial planning to finalization, where did you have to innovate to overcome a challenge?</b></br>

One significant challenge was implementing the SMS notification feature while considering device compatibility and permissions. Innovation was required to ensure the app functioned correctly on devices without telephony capabilities and to handle permissions efficiently. This was achieved by declaring the telephony feature as optional and carefully managing SMS permissions, ensuring functionality without compromising device compatibility.

<br><b>In what specific component from your mobile app were you particularly successful in demonstrating your knowledge, skills, and experience?</b></br>

A particularly successful aspect was the implementation of the inventory management system using RecyclerView and SQLite. This component demonstrated skills in creating a responsive, data-driven UI, and efficient database handling. It showcased an understanding of Android development best practices, effective UI design principles, and robust data management techniques.