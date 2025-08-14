const db=require('../database.js');
const bcrypt = require('bcryptjs');

const accounts={
    // Palauttaa kaikki pankkitilit
    getAll(callBack){
        return db.query('SELECT * FROM accounts', callBack);
    },
    // Palauttaa tietyn pankkitilin tilinumeron perusteella
    getByNumber(number, callBack){
        return db.query('SELECT * FROM accounts WHERE account_number=?',[number], callBack);
    },
    getByCustomerId(id, callBack){
        return db.query('SELECT * FROM accounts WHERE account_customer_id=?',[id], callBack);
    },
    // Lisää uuden pankkitilin
    add(data,callBack){
        return db.query("INSERT INTO accounts (account_number, account_type, account_customer_id, account_state, account_balance) VALUES(?,?,?,?,?)", [data.account_number, data.account_type, data.account_customer_id, data.account_state, data.account_balance],callBack);
    },
    // Päivitetään tietyn pankkitilin tiedot uusilla tiedoilla number:n perusteella
    update(number, data, callBack){
        return db.query('UPDATE accounts SET account_number=?, account_type=?, account_customer_id=?, account_state=?, account_balance=? WHERE account_number=?', [data.account_number, data.account_type, data.account_customer_id, data.account_state, data.account_balance, number], callBack);
    },
    updateCreditLimit(number, data, callBack){
        return db.query(`UPDATE accounts SET account_credit_limit=? WHERE account_number=? AND account_type='credit'`, [-Math.abs(data.credit_limit), number], callBack);
    },
    withdrawBalance(number, amount, callback){
        return db.query(`UPDATE accounts SET account_balance = account_balance - ? WHERE 
        (account_number=? AND account_balance >= ? AND account_type='debit') OR
        (account_number=? AND account_balance - ? >= account_credit_limit AND account_type='credit')`, [amount, /*eka*/  number, amount, /*toka*/ number, amount], callback);
    },
    addBalance(number, amount, callback){
        return db.query(`UPDATE accounts SET account_balance = account_balance + ? WHERE account_number=?`, [amount, number], callback);
    },
    // Poistetaan tietty asiakas number:n perusteella
    delete(number, callback){
        return db.query('DELETE FROM accounts WHERE account_number=?',[number],callback)
    }
}

module.exports=accounts;