//-- limit switch holder / servo arm extender for Auto Bed Leveling
//-- by AndrewBCN - Barcelona, Spain - November 2014
//-- GPLV3
//-- (armv3 below)

//-- remix of Thingiverse #167430 (armv1 below)
//-- by zennmaster

//-- remix of Thingiverse #209384 (armv2 below)
//-- by Stephen Castello

// Some measurements and features

// armv1 has a (fixed) length of 37mm,
// width is 10mm, thickness is 4mm,
// uses 2 x M2.5 screws and nuts to hold a small limit switch.
// Uses standard 9G microservo.

// armv2 is for a much larger limit switch and also
// apparently for a larger servo, although everything seems
// to be configurable. Default thickness is 5mm.
// Uses M3 screws and nuts to hold the limit switch.

// armv3 has a parametric length (default is 37mm), uses
// a T-profile with 4mm base thickness to avoid flexing,
// holds the small limit switch with either 2 x small M2.5
// screws and nuts or a nylon tie.
// Uses standard 9G microservo.
// The slot for the servo arm is properly shaped.


// arm body length, add approximately 8mm for total length
// i.e. with an arm length of 37mm, the distance between
// the servo center and the tip of the limit switch is 45mm

// Configure according to your extruder dimensions


x = 2;
gX = 15; gY = 20; gZ = 2;
gXoff = -2.5;
bX = 10; bY = 15; bZ = 3+3;
bYY = 12.6;

rX = 12;rY=1.1;rZ=1.5;

arm_length=43.5; // [30:70]


translate([0,0,gZ])
{
   difference(){
   armv3();

    translate([-33.5,0,-5])
        cylinder(d=3,h=20,$fn=30);
     
    translate([-10.6,0,-5])
        cylinder(d=1.8,h=20,$fn=30);
     
   }
   translate([-arm_length+1,0,2.1])
   rotate([0,10,0]) cube([2,8,8],true);
 }
/*
difference() {
color("orange")
translate([gXoff*3/2,0,1])
cube([gX+4,gY+4,gZ+4]);

color ("blue") 
translate([0,4.5,0])
cube([bX+ .2,bY+.2,bZ +.2]);
    
translate([gXoff+.1,0,2])
color ("green") cube([gX+.1,gY+.1,gZ+.1]);
}

*/
//translate([gXoff,0,2])
//color ("green") cube([gX,gY,gZ]);



//translate([0,0,bZ])
//mirror([0,0,1])

module sliderMain() {

difference() {
  union() {
  //mirror([0,0,1]) 
    //translate([arm_length,(bY+5)/2,-8])  armv3();
  
    //main body
   color ("blue") 
       translate([0,(gY-bY)/2,0])
       cube([bX,bY,bZ]);
   
    translate([0,10,3.9]) {
   rotate([135,0,0])  translate([0,1.15,-1.15]) cube([bX,5.5,5]);
        
   rotate([318,0,0])  translate([0,-1.40,+1.40]) cube([bX,5.5,5]);
      
   translate([0,(-8)/2,0])
      cube([bX,8,7]);
    }
    
  }
    /* color("pink") translate([20,(bY+5)/2,.25]) hull() {
       
      translate([0,0,5.5]) cylinder(r=3.2,h=.5,center=true,$fn=32);
      translate([-14.7,0,5.5]) cylinder(r=2,h=.5,center=true,$fn=32);
    }
    */
  
    color ("red") {
    //center slot
    translate([-rY,(gY-rY*2)/2,0])
        cube ([rX,rY*2,rZ]);

    //left slot
    translate([-1,(gY-bYY-rY)/2+1.2,0]) 
        rotate([0,0,-5])
        cube ([rX,rY,rZ]);
    //right slot
    translate([-1,(gY+bYY+rY)/2-1.2-rY,0]) 
        rotate([0,0,5]) 
        cube ([rX,rY,rZ]);
    }
    //bottom clearance
    translate([0,(gY-11.2)/2,0]) 
        cube([bX,11.2,.75]); 
    
    /* 
    //slot hole  
    translate([0,(gY-5)/2,2.6]) 
       cube([bX,5.5,2 ]);
    */  
    
    translate([gY/4,gY/2,-5])
        cylinder(d=3,h=20,$fn=30);
    
    //translate([arm_length-10,gY/2,0])
    //    cylinder(d=2,h=10,$fn=30);
}


/*
//slot hole supports
translate([0,(gY-.2)/2+2,2.6]) 
cube([bX,.2,2]);

translate([0,(gY-.2)/2-2,2.6]) 
cube([bX,.2,2]);
*/
}
//translate([bX,(gY-12.6)/2,0]) cube([1,12.6,1]);
//translate([0,(gY-11.5)/2,0]) cube([1,11.5,1]);



module armv3() {
  //  rotate([180,0,0]) 
  difference() {
      // body
      union() {
      cylinder(r=6, h=4,center=true,$fn=32);
      translate([-arm_length/2,0,0]) cube([arm_length,8,4],center=true);


      }
      
    // hole for servo spindle
    cylinder(r=3.8,h=10,center=true,$fn=32); 

    // slot for servo arm
    hull() {
      translate([0,0,5.5]) cylinder(r=3.2,h=10,center=true,$fn=32);
      translate([-14.7,0,5.5]) cylinder(r=2,h=10,center=true,$fn=32);
    }
 
    }

   //rotate([135,0,0])  translate([-arm_length,1.15,-1.15]) cube([bX,5.5,5]);
        
   // rotate([318,0,0])  translate([-arm_length,-1.40,+1.40]) cube([bX,5.5,5]);
}