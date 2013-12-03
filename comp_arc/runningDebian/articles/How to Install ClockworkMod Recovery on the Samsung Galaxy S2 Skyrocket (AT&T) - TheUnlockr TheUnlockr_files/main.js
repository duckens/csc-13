$(function() { 

// Activates the Carousel
$('.carousel').carousel();

// Fixes Comment Input Classes
$("textarea#comment").addClass("input-block-level");
$("input#submit").addClass("btn btn-primary");

// Fixes Dropdown Inputs
$("select").addClass("form-control");

// Fixes Active Class
$(".current-menu-item").addClass("active");

$(".sidebar-ad").parent().parent().addClass("ad");

//Fixes Floating Issue with ROM and Spec Pages
$("#ROMWrapper").after('<p class="clearfix"></p>');
$("#SpecsWrapper").after('<p class="clearfix"></p>');

//Makes Current YouTube Videos Responsive
$(".embed-youtube").addClass("flex-object");

// Makes Images Responsive
$("img").addClass("img-responsive");

// Make All Forms Bootstrap Compatible
$("textarea").addClass("form-control");
$("input").addClass("form-control");
$(":submit").addClass("btn btn-primary");

});