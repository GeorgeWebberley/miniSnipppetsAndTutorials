const express = require("express");
// get the router method from express
const router = express.Router();
const Book = require("../models/book");

router.get("/", async (req, res) => {
  let books;
  try {
    // first find all books, then sort desc, then limit to 10 most recent, thene execute query
    books = await Book.find()
      .sort({ createdAt: "desc" })
      .limit(10)
      .exec();
  } catch (err) {
    // because books query returns an array
    books = [];
  }
  res.render("index", {
    books: books
  });
});

module.exports = router;
