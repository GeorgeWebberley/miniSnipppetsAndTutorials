const express = require("express");
// get the router method from express
const router = express.Router();
// multer allows us to deal with image objects and save them??
const multer = require("multer");
// Allows us to use path.join to make the path for our image files (see uploadPath)
const path = require("path");
const fs = require("fs");
const imageMimeTypes = ["image/jpeg", "image/png", "images/gif"];
// this is the schema (or class?) which is why it is in caps?
const Book = require("../models/book");
const Author = require("../models/author");

// Multer will automatically make the folders for us :)
const uploadPath = path.join("public", Book.coverImageBasePath);

// multer config
const upload = multer({
  dest: uploadPath,
  // allows us to specify what files we will accept.
  fileFilter: (req, file, callback) => {
    // first argument is for error, second is a boolean.
    // The boolean is checking whether the file type is in the imageMimeTypes array
    callback(null, imageMimeTypes.includes(file.mimetype));
  }
});

// All Books
router.get("/", async (req, res) => {
  // notice we didnt pass any parameters to .find()
  // this returns us a query object that we can build on and execute later
  let query = Book.find();
  if (req.query.title != null && req.query.title != "") {
    query = query.regex("title", new RegExp(req.query.title, "i"));
  }
  if (req.query.publishedBefore != null && req.query.publishedBefore != "") {
    // lte is less than or equal to.
    // Basically saying if the published date is less than or before the publishedBefore then update 'query'
    query = query.lte("publishedDate", req.query.publishedBefore);
  }
  if (req.query.publishedAfter != null && req.query.publishedAfter != "") {
    query = query.gte("publishedDate", req.query.publishedAfter);
  }
  try {
    // now we execute the query
    const books = await query.exec();
    res.render("books/index", {
      books: books,
      searchOptions: req.query
    });
  } catch (err) {
    res.redirect("/");
  }
});

// New Book
router.get("/new", async (req, res) => {
  renderNewPage(res, new Book());
});

// Create Book
// tells multer we are uploading a single file with the name of cover
// multer does all the work behind the scenes to take the image and save in the right file (specified at the top)
router.post("/", upload.single("cover"), async (req, res) => {
  const fileName = req.file != null ? req.file.filename : null;
  console.log("before putting data");
  const book = new Book({
    title: req.body.title,
    author: req.body.author,
    // Have to put inside 'Date' because req.body returns the string which then needs to be converted.
    publishDate: new Date(req.body.publishDate),
    pageCount: req.body.pageCount,
    // if no file uploaded then it will be null
    coverImageName: fileName,
    description: req.body.description
  });
  try {
    const newBook = await book.save();
    res.redirect("books/${newBook.id}");
  } catch (err) {
    console.log(err);
    if (book.coverImageName != null) {
      // we need this function because the image of the file gets
      // uploaded regardless of if the other fields are filled out correctly.
      removeBookCover(book.coverImageName);
    }
    renderNewPage(res, book, true);
  }
});

function removeBookCover(fileName) {
  // unlink here removes the file
  fs.unlink(path.join(uploadPath, fileName), err => {
    if (err) {
      console.log(err);
    }
  });
}

// Update Book
router.put("/:id", upload.single("cover"), async (req, res) => {
  const fileName = req.file != null ? req.file.filename : null;
  let book;
  let oldBookCover;
  try {
    book = await Book.findById(req.params.id);
    oldBookCover = book.coverImageName;
    book.title = req.body.title;
    book.author = req.body.author;
    // we specify new Date here which converts a string (req.body.publishDate) to a date
    book.publishDate = new Date(req.body.publishDate);
    book.pageCount = req.body.pageCount;
    book.description = req.body.description;
    if (req.file != null && req.file !== "") {
      console.log("HELLO");
      removeBookCover(book.coverImageName);
      book.coverImageName = fileName;
    }
    await book.save();
    res.redirect(`/books/${book.id}`);
  } catch {
    if (book != null && oldBookCover != null) {
      if (book.coverImageName != oldBookCover) {
        removeBookCover(book.coverImageName);
        book.coverImageName = oldBookCover;
      }
      renderEditPage(res, book, true);
    } else {
      res.redirect("/");
    }
  }
});

// Show Book
router.get("/:id", async (req, res) => {
  try {
    // findById will just get the ID of the author but not the author name etc. therefore we use 'populate'
    // function to get other information (in this case all the author information)
    const book = await Book.findById(req.params.id)
      .populate("author")
      .exec();
    res.render("books/show", { book: book });
  } catch {
    res.redirect("/");
  }
});

// Edit Book
router.get("/:id/edit", async (req, res) => {
  try {
    const book = await Book.findById(req.params.id);
    renderEditPage(res, book);
  } catch {
    res.redirect("/");
  }
});

// Delete Book
router.delete("/:id", async (req, res) => {
  let book;
  try {
    book = await Book.findById(req.params.id);
    removeBookCover(book.coverImageName);
    await book.remove();
    res.redirect("/books");
  } catch (err) {
    console.log(err);
    if (book != null) {
      res.render("books/show", {
        book: book,
        errorMessage: "Could not remove book"
      });
    } else {
      res.redirect("/");
    }
  }
});

async function renderNewPage(res, book, hasError = false) {
  renderFormPage(res, book, "new", hasError);
}

async function renderEditPage(res, book, hasError = false) {
  renderFormPage(res, book, "edit", hasError);
}

async function renderFormPage(res, book, form, hasError = false) {
  try {
    const authors = await Author.find({});
    const params = {
      authors: authors,
      book: book
    };
    if (hasError) {
      if (form === "edit") {
        params.errorMessage = "Error Updating Book";
      } else {
        params.errorMessage = "Error Creating Book";
      }
    }
    res.render(`books/${form}`, params);
  } catch (err) {
    res.redirect("/books");
  }
}

module.exports = router;
