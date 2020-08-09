// if we are not in production we want to load our local environment variables from the dot file .env
if (process.env.NODE_ENV !== "production") {
  require("dotenv").config();
}

const express = require("express");
const app = express();
const expressLayouts = require("express-ejs-layouts");
// Body parser allows us easily to get information from forms etc?
const bodyParser = require("body-parser");

// takes a relevant path
const indexRouter = require("./routes/index");
const authorRouter = require("./routes/authors");
const bookRouter = require("./routes/books");

// method-override allows us to use put and delete methods.
const methodOverride = require("method-override");

// sets the view engine to ejs
app.set("view engine", "ejs");
// Tells the app where views folder is (So dont have to keep typing /views/file)
app.set("views", __dirname + "/views");

// sets the 'layout' (i.e. the html template which contains header, footer etc) to the layout file in layouts folder
app.set("layout", "layouts/layout");
// explicityly declares we will be useing express-ejs-layouts
app.use(expressLayouts);
// method override basically reads the url and uses that to trigger a put or delete request.
// Therefore we need to feed it something that would unlikely be a form input (in this case '_method')
app.use(methodOverride("_method"));
// tells express where our public files will be (i.e. in a folder called public)
app.use(express.static("public"));
// We increase the limit of size of information to 10mb so that later we can upload files
app.use(bodyParser.urlencoded({ limit: "10mb", extended: false }));

const mongoose = require("mongoose");

mongoose.connect(process.env.DATABASE_URL, {
  useNewUrlParser: true,
  useUnifiedTopology: true
});
const db = mongoose.connection;
// 'on' is like an event listener. So if error...
db.on("error", error => console.error(error));
// 'once' means it will only do it once (in this case, upon opening database for the first time)
db.once("open", () => console.log("Connected to Mongoose..."));

// set the index to be on '/'
app.use("/", indexRouter);
// this basically means that everything in our authors route will be preceded with /authors (e.g. '/' will be '/authors/')
app.use("/authors", authorRouter);
app.use("/books", bookRouter);

// listen on port (either environemnt variable if set or 3000. When deployed, the server sets the environment variable for us).
app.listen(process.env.PORT || 3000);
