const db=require('../database.js');
const bcrypt = require('bcryptjs');

const accounts={
    // Palauttaa kaikki tilitapahtumat
    getAll(callBack){
        return db.query('SELECT * FROM transactions ORDER BY transaction_date DESC', callBack);
    },
    // Palauttaa tietyn tilitapahtuman ID:n perusteella
    getById(id, callBack){
        return db.query('SELECT * FROM transactions WHERE transaction_id=?',[id], callBack);
    },
    // Palauttaa tilitapahtumat accountin numeron perusteella
    getByAccountNumber(number, callback){
        return db.query('SELECT * FROM transactions WHERE (transaction_sender=? OR transaction_receiver=?) ORDER BY transaction_date DESC',[number, number], callback);
    },
    // Palauttaa tilitapahtumat accountin numeron ja/tai tyypin perusteella
    getByAccountNumberAndType(number, type, callback){
        return db.query('SELECT * FROM transactions WHERE (transaction_sender=? OR transaction_receiver=?) AND transaction_type=? ORDER BY transaction_date DESC',[number, number, type], callback);
    },
    // Lisää uuden tilitapahtuman
    add(receiver, sender, amount, type, callBack){
        return db.query("INSERT INTO transactions (transaction_receiver, transaction_sender, transaction_amount, transaction_type) VALUES(?,?,?,?)", [receiver, sender, amount, type],callBack);
    },
    // Päivitetään tietyn tilitapahtuman tiedot uusilla tiedoilla ID:n perusteella
    update(id, data, callBack){
        return db.query('UPDATE transactions SET transaction_receiver=?, transaction_sender=?, transaction_amount=? WHERE transaction_id=?', [data.receiver, data.sender, data.amount, id], callBack);
    },
    // Poistetaan tietty tilitapahtuma ID:n perusteella
    delete(id, callback){
        return db.query('DELETE FROM transactions WHERE transaction_id=?',[id],callback)
    },
    deleteAll(callback){
        return db.query('DELETE FROM transactions', callback);
    }


}

module.exports=accounts;//