const db=require('../database.js');
const bcrypt = require('bcryptjs');

const accounts={
    // Palauttaa kaikki asiakkaat
    getAll(callBack){
        return db.query('SELECT * FROM customers', callBack);
    },
    // Palauttaa tietyn asiakkaan ID:n perusteella
    getById(id, callBack){
        return db.query('SELECT * FROM customers WHERE customer_id=?',[id], callBack);
    },
    // Lisää uuden asiakkaan
    add(data,callBack){
        // Tämä on opettajan esimerkistä miten pin koodi turvataan
        //          mikä hashataan   kuin paljon hashataan 
        //bcrypt.hash(newUser.password, 10,function(err,hashed_password){
        //    return db.query(`INSERT INTO accounts VALUES(?,?,?)`,[newUser.fname,newUser.lname,hashed_password],callBack);
        //})
        return db.query("INSERT INTO customers (customer_fname, customer_lname, customer_address, customer_phone_number) VALUES(?,?,?, ?)", [data.fname, data.lname, data.address, data.phone_number],callBack);
    },

    // Päivitetään tietyn asiakkaan tiedot uusilla tiedoilla ID:n perusteella
    update(id, data, callBack){
        return db.query('UPDATE customers SET customer_fname=?, customer_lname=?, customer_address=? WHERE customer_id=?', [data.fname, data.lname, data.address, id], callBack);
    },
    // Poistetaan tietty asiakas ID:n perusteella
    delete(id, callback){
        return db.query('DELETE FROM customers WHERE customer_id=?',[id],callback)
    },
}

module.exports=accounts;//