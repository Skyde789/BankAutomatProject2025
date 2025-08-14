var express = require('express');
var router = express.Router();
var transactions = require('../models/transactions_model');
var accounts = require('../models/accounts_model');
const { getErrorResponse } = require('../utils/status_codes');
const { getSuccessResponse } = require('../utils/status_codes');

/* GET transactions listing. */
// Kutsuu transactions_model tiedostosta getAll funktiota. Tämä on postmanissa: "localhost:3000/transactions/"
router.get('/', function(request, response){
    transactions.getAll(function(err,result){
        if(err)
            return response.json(getErrorResponse(1300));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(1308));
        else
            response.json(result);
    })
});
// Kutsuu transactions_model tiedostosta getByID funktiota. Tämä on postmanissa: "localhost:3000/transactions/3"
router.get('/:id',function(request,response){
    transactions.getById(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(1301));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(1308));
        else
            response.json(result[0]);

    })
});

router.get('/account/:id',function(request,response){
    accounts.getByNumber(request.params.id, function(err, accountResult) {
        if(err || !accountResult || accountResult.length == 0)
            return response.json(getErrorResponse(1309));

        transactions.getByAccountNumber(request.params.id,function(err,result){
            if(err)
                return response.json(getErrorResponse(1301));
            else if (!result || result.length == 0)
                return response.json(getErrorResponse(1308));
            else
                response.json(result);
        });
    });
});

router.get('/account/:type/:id',function(request,response){
    accounts.getByNumber(request.params.id, function(err, accountResult) {
        if(err || !accountResult || accountResult.length == 0)
            return response.json(getErrorResponse(1309));

        transactions.getByAccountNumberAndType(request.params.id, request.params.type, function(err,result){
            if(err)
                return response.json(getErrorResponse(1307));
            else
                response.json(result);
        });
    });
});
// Kutsuu transactions_model tiedostosta add funktiota. Tämä on postmanissa: "localhost:3000/transactions/" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.post('/', function(request,response){
    transactions.add(request.body,function(err,result){
        if (err)
            return response.json(getErrorResponse(1302));
        else 
            response.json(getSuccessResponse(130));
    })
});
// Kutsuu transactions_model tiedostosta update funktiota. Tämä on postmanissa: "localhost:3000/transactions/3" ja lisänä Body osioon lisättynä { Key: fname Value: Pentti } jne jne
router.put('/:id', function(request,response){
    transactions.update(request.params.id, request.body, function(err,result){
        if (err)
            return response.json(getErrorResponse(1303));
        else 
            response.json(getSuccessResponse(131));
    })
});
// Kutsuu transactions_model tiedostosta delete funktiota. Tämä on postmanissa: "localhost:3000/transactions/3"
router.delete('/:id', function(request, response){
    transactions.delete(request.params.id, function(err,result){
        if (err)
            return response.json(getErrorResponse(1304));
        else 
            response.json(getSuccessResponse(132));
    })
});
router.delete('/', function(request, response){
    transactions.deleteAll(function(err,result){
        if (err)
            return response.json(getErrorResponse(1305));
        else 
            response.json(getSuccessResponse(133));
    })
});
// Tries to deposit balance from Accounts_model and if succeeded, creates a new entry to transactions
router.post('/deposit/', function(request,response){
    const account = request.body.account;
    const amount = request.body.amount;
    const type = request.body.type;

    // Get the account information
    accounts.getByNumber(account, function(err, accountResult) {
        if (err || !accountResult || accountResult.length === 0) 
            return response.json(getErrorResponse(1000));

        const accountData = accountResult[0];
        if (request.user&&accountData.account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(1310));

        if (accountData.account_state == 'locked')
            return response.json(getErrorResponse(1001));

        accounts.addBalance(account, amount, function(err,result){
            if (err)
                return response.json(getErrorResponse(1002));
                
            // Create a transaction record to the transactions table
            transactions.add(account, account, amount, type, function(err, result) {
               if (err) {
                    return response.json(getErrorResponse(1003));
                } else {
                    return response.json(getSuccessResponse(100));
                }
            });  
        });
    });

    
});

// Tries to withdraw balance from Accounts_model and if succeeded, creates a new entry to transactions
router.post('/withdraw/', function(request,response){
    const account = request.body.account;
    const amount = request.body.amount;
    const type = request.body.type;

    // Get the account information
    accounts.getByNumber(account, function(err, accountResult) {
        if (err || !accountResult || accountResult.length === 0) 
            return response.json(getErrorResponse(1100));

        const accountData = accountResult[0];
        if (request.user&&accountData.account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(1310));

        if (accountData.account_state == 'locked')
            return response.json(getErrorResponse(1101));

        // Check if the account account has the balance required to withdraw
        if (accountData.account_type == 'debit') {
            if (accountData.account_balance < amount) {
                return response.json(getErrorResponse(1102));
            }
        } 
        // Credit accounts need to take credit limits into consideration compared to debit accounts
        else if (accountData.account_type == 'credit') {
            const newBalance = accountData.account_balance - amount;

            if (newBalance < accountData.account_credit_limit) {
                return response.json(getErrorResponse(1102));
            }
        }
        
        // Try to withdraw balance from the account
        accounts.withdrawBalance(account, amount, function(err, accountBalanceResult) {
            if (err) {
                return response.json(getErrorResponse(1104));
            }
            // Create a transaction record to the transactions table
            transactions.add(account, account, amount, type, function(err, result) {
                if (err) {
                    return response.json(getErrorResponse(1105));
                } else {
                     return response.json(getSuccessResponse(110));
                }
            });
        });  
    });
});

// Tries to transfer balance from Accounts_model and if succeeded, creates a new entry to transactions
router.post('/transfer/', function(request, response) {
    const receiver = request.body.receiver;
    const sender = request.body.sender;
    const amount = request.body.amount;
    const type = request.body.type;

    if (receiver == sender)
        return response.json(getErrorResponse(1207));
    // Get the account information
    accounts.getByNumber(sender, function(err, accountResult) {
        if (err || !accountResult || accountResult.length === 0) {
            return response.json(getErrorResponse(1200));
        }

        const accountData = accountResult[0];
        if (request.user&&accountData.account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(1310));
        
        if (accountData.account_state == 'locked')
            return response.json(getErrorResponse(1201));

        // Check if the account account has the balance required to transfer
        if (accountData.account_type == 'debit') {
            if (accountData.account_balance < amount) {
                return response.json(getErrorResponse(1202));
            }
        } 
        // Credit accounts need to take credit limits into consideration compared to debit accounts
        else if (accountData.account_type == 'credit') {
            const newBalance = accountData.account_balance - amount;

            if (newBalance < accountData.account_credit_limit) {
                return response.json(getErrorResponse(1202));
            }
        } 

        // Get the receivers information
        accounts.getByNumber(receiver, function(err, receiverResult) {
            if (err || !receiverResult || receiverResult.length === 0) {
                return response.json(getErrorResponse(1203));
            }

            // Try to withdraw balance from the account
            accounts.withdrawBalance(sender, amount, function(err, accountBalanceResult) {
                if (err) {
                    return response.json(getErrorResponse(1204));
                }

                // Try to add balance to the receiver
                accounts.addBalance(receiver, amount, function(err, receiveBalanceResult) {
                    if (err) {
                        return response.json(getErrorResponse(1205));
                    }

                    // Create a transaction record to the transactions table
                    transactions.add(receiver, sender, amount, type, function(err, result) {
                        if (err) {
                            return response.json(getErrorResponse(1206));
                        } else {
                            return response.json(getSuccessResponse(120));
                        }
                    });
                });
            });
        });
    });
});
module.exports = router;