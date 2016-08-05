x = 2;
gX = 15; gY = 20; gZ = 2;
gXoff = -2.5;
bX = 10; bY = 15; bZ = 3+3;
bYY = 12.6;

rX = 12;rY=1.1;rZ=1.5;

arm_length=43.5; // [30:70]

translate([0,0,11])
mirror([0,0,1])
sliderMain() ;




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
        cylinder(d=3.2,h=20,$fn=30);
    
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