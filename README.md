# Bank Automat Simulator L16

## Project Overview
This project is a bank automat (ATM) simulator designed to replicate real-world ATM functionality. It features a user-friendly frontend built with Qt Creator and C++, and a backend powered by Node.js (Express) connected to a MySQL database via a REST API.

## Features
- User login with idle timeout
- Account types: credit, debit, and dual card options
- View account transaction history (custom buttons)
- Deposit, withdraw, and transfer funds
- Custom interface elements
- Secure frontend-backend communication

## Technologies Used
- **Frontend**: Qt Creator (Qt Widgets) with C++
- **Backend**: Node.js, Express
- **Database**: MySQL
- **API**: RESTful services for frontend-backend communication

## Installation

### Prerequisites
- Qt Creator
- Visual Studio Code (or another Node.js IDE)
- MySQL Server
- Node.js and npm installed

### Setup Instructions
1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/bank-automat-simulator.git
   ```
2. **Backend setup:**

-Navigate to the backend folder.

-Open the Command Prompt and install dependencies:

```bash
npm install
```
-Configure your MySQL connection settings.

-Start the server by running:

```bash
npm start
```
3.**Frontend setup:**

-Open the frontend project in Qt Creator.

-Build and run the project.

4.**Database setup:**

-Import the provided SQL file into your MySQL server to create the necessary tables and seed data.

5.**Usage**

-Start the backend server.

-Run the frontend application.

-Interact with the ATM simulator: insert card, enter PIN, manage accounts, and perform transactions.


## Authors

-Sami Laihiainen https://github.com/Skyde789

-Aleksi Vainikka https://github.com/Kurkkuu

-Niko Laine https://github.com/JylleRo

-Oona Kujala https://github.com/oonaku
