const db=require('../database.js');
const bcrypt = require('bcryptjs');

const accounts={
    // Palauttaa kaikki pankkikortit
    getAll(callBack){
        return db.query('SELECT * FROM cards', callBack);
    },
    // Palauttaa tietyn pankkikortin korttinumeron perusteella
    getByNumber(number, callBack){
        return db.query('SELECT * FROM cards WHERE card_number=?',[number], callBack);
    },
    // Lisää uuden pankkikortin ja salaa PIN-koodin
    add(data,callBack){
        // Tämä on opettajan esimerkistä miten pin koodi turvataan
        //          mikä hashataan   kuin paljon hashataan 
        bcrypt.hash(data.card_pin, 10,function(err,hashed_pin){
            return db.query(`INSERT INTO cards (card_number, card_pin, card_account_number, card_owner_id) VALUES(?,?,?,?)`,[data.card_number, hashed_pin, data.card_account_number, data.card_owner_id],callBack);
        })
    },
    checkPin(number, callBack){
        return db.query('SELECT card_pin FROM cards WHERE card_number=?', [number], callBack);
    },
    // Lisää kaksoiskortin
    addDouble(data, callBack){
        bcrypt.hash(data.card_pin, 10,function(err,hashed_pin){
        return db.query("INSERT INTO cards (card_number, card_pin, card_owner_id, card_account_number, card_account_number2, card_type) VALUES(?, ?, ?, ?, ?, ?)", [data.card_number, hashed_pin, data.card_owner_id, data.card_account_number, data.card_account_number2, "double"], callBack);
        })
    }, 
    //Päivittää tietyn kortin tiedot korttinumeron perusteella
    update(number, data, callBack){
        return db.query('UPDATE cards SET card_number=?, card_account_number=?, card_owner_id=? WHERE card_number=?', [data.card_number, data.card_account_number, data.card_owner_id, number], callBack);
    },
    // Poistaa tietyn pankkikortin korttinumeron perusteella
    delete(number, callback){
        return db.query('DELETE FROM cards WHERE card_number=?',[number],callback)
    },
    // Päivittää pankkikortin yritykset
    setAttempts(number, attempts, callback){
        return db.query('UPDATE cards SET card_attempts=? WHERE card_number=?',[attempts, number],callback)
    },
    // Päivittää kortin statuksen joko "Active" tai "Locked"
    updateStatus(number, statusupdate, callback){
        return db.query('UPDATE cards SET card_state=? WHERE card_number=?',[statusupdate, number],callback)
    },
}

module.exports=accounts;//