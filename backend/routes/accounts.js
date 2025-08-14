var express = require('express');
var router = express.Router();
var accounts = require('../models/accounts_model');
const { getErrorResponse } = require('../utils/status_codes');
const { getSuccessResponse } = require('../utils/status_codes');
/* GET accounts listing. */
// Kutsuu accounts_model tiedostosta getAll funktiota. Tämä on postmanissa: "localhost:3000/accounts/"
router.get('/', function(request, response){
    accounts.getAll(function(err,result){
        if(err)
            return response.json(getErrorResponse(2000));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(2007));
        else
            response.json(result);
    })
});
// Kutsuu accounts_model tiedostosta getByID funktiota. Tämä on postmanissa: "localhost:3000/accounts/3"
router.get('/:id',function(request,response){
    accounts.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(2001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(2007));
        else if (request.user && result[0].account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(5003)); 
        else
            response.json(result[0]);
    });
});

// Kutsuu accounts_model tiedostosta getByID funktiota. Tämä on postmanissa: "localhost:3000/accounts/3"
router.get('/check/:id',function(request,response){
    accounts.getByNumber(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(2001));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(2007));
        else
            response.json(getSuccessResponse(204));
    });
});

router.get('/customer/:id',function(request,response){
    accounts.getByCustomerId(request.params.id,function(err,result){
        if(err)
            return response.json(getErrorResponse(2002));
        else if (!result || result.length == 0)
            return response.json(getErrorResponse(2007));
        else if (request.user && request.params.id != request.user.owner_id)
            return response.json(getErrorResponse(5003));
        else
            response.json(result);
    })
});
// Kutsuu accounts_model tiedostosta add funktiota. Tämä on postmanissa: "localhost:3000/accounts/" ja lisänä Body osioon lisättynä { Key: account Value: number } jne jne
router.post('/', function(request,response){
    accounts.add(request.body,function(err,result){
        if (err)
            return response.json(getErrorResponse(2008));
        else 
            response.json(getSuccessResponse(200));
    })
});
// Kutsuu accounts_model tiedostosta update funktiota. Tämä on postmanissa: "localhost:3000/accounts/3" ja lisänä Body osioon lisättynä { Key: account Value: number } jne jne
router.put('/:id', function(request,response){
    accounts.getByNumber(request.params.id,function(err,accountResult){
        if(err)
            return response.json(getErrorResponse(2001));
        else if (!accountResult || accountResult.length == 0)
            return response.json(getErrorResponse(2007));
        else if (request.user && accountResult[0].account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(5003));

        accounts.update(request.params.id, request.body, function(err,result){
            if (err)
                return response.json(getErrorResponse(2005));
            else 
                response.json(getSuccessResponse(201));
        });
    });
    
});
router.put('/credit/:id', function(request,response){
    accounts.getByNumber(request.params.id,function(err,accountResult){
        if(err)
            return response.json(getErrorResponse(2001));
        else if (!accountResult || accountResult.length == 0)
            return response.json(getErrorResponse(2007));
        else if (request.user && accountResult[0].account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(5003));
        const accountData = accountResult[0];
        if (accountData.account_state == 'locked')
            return response.json(getErrorResponse(2003));

        accounts.updateCreditLimit(request.params.id, request.body, function(err,result){
            if (err)
                return response.json(getErrorResponse(2005));
            else 
                response.json(getSuccessResponse(203));
        })
    });
});
// Kutsuu accounts_model tiedostosta delete funktiota. Tämä on postmanissa: "localhost:3000/accounts/3"
router.delete('/:id', function(request, response){
    accounts.getByNumber(request.params.id,function(err,accountResult){
        if(err)
            return response.json(getErrorResponse(2001));
        else if (!accountResult || accountResult.length == 0)
            return response.json(getErrorResponse(2007));
        else if (request.user && accountResult[0].account_customer_id != request.user.owner_id)
            return response.json(getErrorResponse(5003));
        accounts.delete(request.params.id, function(err,result){
            if (err)
                return response.json(getErrorResponse(2006));
            else 
                response.json(getSuccessResponse(202));
        });
    });
});

module.exports = router;