const express = require("express");
// get the router method from express
const router = express.Router();

// this is the schema (or class?) which is why it is in caps?
const Author = require("../models/author");
const Book = require("../models/book");

// All Authors Route
router.get("/", async (req, res) => {
  let searchOptions = {};
  // get requests send through the query string in the url. Therefore we use req.query
  // (instead of req.body as we do in a post request.)
  if (req.query.name != null && req.query.name !== "") {
    // RegExp here allows us to search even if not exact (i.e. if user types geo then george will still be found)
    // 'i' flag means case insensitive
    searchOptions.name = new RegExp(req.query.name, "i");
  }
  try {
    // we pass it our searchOptions variable (which is like a 'where' statement when querying)
    const authors = await Author.find(searchOptions);
    res.render("authors/index", {
      authors: authors,
      searchOptions: req.query
    });
  } catch (err) {
    res.redirect("/");
  }
});

// New Author Route
router.get("/new", (req, res) => {
  // the second argument (the object) are variables that we want to pass into our ejs file
  res.render("authors/new", { author: new Author() });
});

// Create Author Route
router.post("/", async (req, res) => {
  const author = new Author({
    // we can use this syntax because of body-parser
    name: req.body.name
  });
  // We use try/catch to catch any errors with creating the author
  try {
    // everything in mongodb is done asynchronously. Therefore we need to use await to wait for it to complete the save.
    const newAuthor = await author.save();
    res.redirect(`authors/${newAuthor.id}`);
  } catch (err) {
    // if error, we load the previous page with the variables sent back.
    res.render("authors/new", {
      author: author,
      errorMessage: "Error creating Author"
    });
  }
});

// get one author
router.get("/:id", async (req, res) => {
  try {
    const author = await Author.findById(req.params.id);
    const books = await Book.find({ author: author.id })
      .limit(6)
      .exec();
    res.render("authors/show", {
      author: author,
      booksByAuthor: books
    });
  } catch {
    res.redirect("/");
  }
});

// the edit author page
router.get("/:id/edit", async (req, res) => {
  try {
    // findById is built into mongoose library
    const author = await Author.findById(req.params.id);
    res.render("authors/edit", { author: author });
  } catch (err) {
    res.redirect("/authors");
  }
});

// ------- put and delete are not included with node??? Need to install method-override
// ------- that is how we can differentiate between the below urls and the get url

// update an author (following edit on the edit page?)
router.put("/:id", async (req, res) => {
  let author;
  // We use try/catch to catch any errors with creating the author
  try {
    author = await Author.findById(req.params.id);
    author.name = req.body.name;
    await author.save();
    res.redirect(`/authors/${author.id}`);
  } catch (err) {
    if (author == null) {
      res.redirect("/");
    } else {
      // if error, we load the previous page with the variables sent back.
      res.render("authors/new", {
        author: author,
        errorMessage: "Error updating Author"
      });
    }
  }
});

// Delete an author
router.delete("/:id", async (req, res) => {
  let author;
  try {
    author = await Author.findById(req.params.id);
    await author.remove();
    res.redirect(`/authors`);
  } catch (err) {
    if (author == null) {
      res.redirect("/");
    } else {
      console.log("author id: " + author.id);
      res.redirect(`/authors/${author.id}`);
    }
  }
});

module.exports = router;
