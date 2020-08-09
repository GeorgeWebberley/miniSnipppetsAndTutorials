const mongoose = require("mongoose");
const Book = require("./book");

const authorSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true
  }
});

// pre allows us to run a method before a certain action occurs
// In this case it will run this method when "remove" is called (but before remove occurs)
authorSchema.pre("remove", function(next) {
  Book.find({ author: this.id }, (err, books) => {
    if (err) {
      next(err);
    } else if (books.length > 0) {
      next(new Error("This authors has books still"));
    } else {
      // no parameters here tells mongoose that all is good and we can remove the author.
      next();
    }
  });
});

// First argument is the name of the model, second argument is the schema itself
module.exports = mongoose.model("Author", authorSchema);
