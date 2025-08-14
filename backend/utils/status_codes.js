const errorMessages = {
    // -------------- Transactions --------------
    // [DEPOSIT]
    1000: "Account not found",
    1001: "Account locked",
    1002: "Error adding balance to account",
    1003: "Error creating transaction record",
  
    // [WITHDRAW]
    1100: "Account not found",
    1101: "Account locked",
    1102: "Account has insufficient balance",
    1103: "Error withdrawing from account",
    1104: "Error creating transaction record",
  
    // [TRANSFER]
    1200: "Sender not found",
    1201: "Sender's account locked",
    1202: "Sender has insufficient balance",
    1203: "Receiver not found",
    1204: "Error withdrawing from account",
    1205: "Error adding balance to account",
    1206: "Error creating transaction record",
    1207: "Can't transfer to the same account",
  
    // [GENERIC]
    1300: "Couldn't get all transactions",
    1301: "Couldn't get a transaction by account ID",
    1302: "Couldn't add a new transaction",
    1303: "Couldn't update transactions",
    1304: "Couldn't delete transactions by ID",
    1305: "Couldn't delete all transactions",
    1306: "Couldn't get transactions by account ID",
    1307: "Couldn't get transactions by account ID or transaction type",
    1308: "No transactions found",
    1309: "No account found",
    1310: "Unauthorized access",
  
    // ----------- Account -----------
    2000: "Couldn't get all accounts",
    2001: "Couldn't find account by number",
    2002: "Could not find account by customer ID",
    2003: "Account is locked",
    2004: "Couldn't update credit limit",
    2005: "Couldn't update account",
    2006: "Couldn't delete account",
    2007: "No account found",
    2008: "Couldn't create an account",
  
    // ----------- Customer -----------
    3000: "Couldn't get all customers",
    3001: "Couldn't get a customer by ID",
    3002: "Couldn't add a customer",
    3003: "Couldn't update a customer by ID",
    3004: "Couldn't delete a customer by ID",
    3005: "No customers found",
  
    // ------ Cards ------
    4000: "Couldn't get all cards",
    4001: "Couldn't get a card by number",
    4002: "Couldn't add a new card",
    4003: "Couldn't update card",
    4004: "Couldn't delete card by number",
    4005: "Couldn't update card attempts",
    4006: "No cards found",
    4007: "Couldn't set card status to locked",
    4008: "Couldn't update card status",
    4009: "Card locked",

    // ------- Login ------
    5000: "Could not find card by number",
    5001: "Wrong pin number",
    5002: "Card number or pin missing",
    5003: "Unauthorized access"

};
const successMessages = {
  // -------------- Transactions --------------
  // [DEPOSIT]
  100: "Deposit success",

  // [WITHDRAW]
  110: "Withdraw success",
  
  // [TRANSFER]
  120: "Transfer success",
 
  // [GENERIC]
  130: "Transaction added successfully",
  131: "Transaction updated successfully",
  132: "Transaction deleted successfully",
  133: "Transactions deleted successfully",
  // ----------- Account -----------
  200: "Account added successfully",
  201: "Account updated successfully",
  202: "Account deleted successfully",
  203: "Account credit limit updated successfully",
  204: "Account found",
 
  // ----------- Customer -----------
  300: "Customer added successfully",
  301: "Customer updated successfully",
  302: "Customer deleted successfully",
  // ------ Cards ------
  400: "Card added successfully",
  401: "Double card updated successfully",
  402: "Card updated successfully",
  403: "Card deleted successfully",
  405: "Card status updated successfully",

  // ------ Login ------
  500: "Logged in successfully",
  501: "Card has been locked",

};
function getErrorResponse(code) {
    return {
      errorCode: code,
      message: errorMessages[code] || "Unknown error"
    };
}
function getSuccessResponse(code) {
  return {
    successCode: code,
    message: successMessages[code] || "Unknown success"
  };
}


module.exports = { getErrorResponse, getSuccessResponse };
