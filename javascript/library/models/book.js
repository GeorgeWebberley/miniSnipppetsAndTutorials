const mongoose = require("mongoose");

const path = require("path");

const coverImageBasePath = "uploads/bookCovers";

const bookSchema = new mongoose.Schema({
  title: {
    type: String,
    required: true
  },
  description: {
    type: String
  },
  publishDate: {
    type: Date,
    required: true
  },
  pageCount: {
    type: Number,
    required: true
  },
  createdAt: {
    type: Date,
    required: true,
    default: Date.now
  },
  coverImageName: {
    type: String,
    required: true
  },
  author: {
    // tells mongoose to reference another object inside our collections
    type: mongoose.Schema.Types.ObjectId,
    required: true,
    // referencing the 'Author' collection
    ref: "Author"
  }
});

// creates a 'virtual' property which can act the same as other attributes in Book
// We use a normal function and not arrow function so that we can use 'this' inside the function
bookSchema.virtual("coverImagePath").get(function() {
  if (this.coverImageName != null) {
    return path.join("/", coverImageBasePath, this.coverImageName);
  }
});

// First argument is the name of the model, second argument is the schema itself
module.exports = mongoose.model("Book", bookSchema);
// this exports it as a named variable called coverImageBasePath
module.exports.coverImageBasePath = coverImageBasePath;
