const home = document.querySelector('.home');
const gallery = document.querySelector('.gallery');
const about = document.querySelector('.home');
const contact = document.querySelector('.home');
const bookings = document.querySelector('.home');
const faq = document.querySelector('.FAQ');

window.addEventListener("load", function() {
  if(window.location.href.indexOf('home') > 0){
    home.classList.add('active');
    gallery.classList.remove('active');
    about.classList.remove('active');
    contact.classList.remove('active');
    bookings.classList.remove('active');
    faq.classList.remove('active');
  }else if(window.location.href.indexOf('explore') > 0){
    home.classList.remove('active');
    gallery.classList.add('active');
    about.classList.remove('active');
    contact.classList.remove('active');
    bookings.classList.remove('active');
    faq.classList.remove('active');
  }
});
