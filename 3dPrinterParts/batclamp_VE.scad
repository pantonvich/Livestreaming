tol=.3;
hX=40.0;
hY=30.3;
hZ=5;
th=2;
bth=4;
brktX = 15;
//brktHt = 8.7;
brktLen=39-7;

yZ = 12.5;
yY = 32.5;
yX = 18.1; //15.4;

ycY= 22.65+.1;


screwDia = 2;
screwDiaClamp=2.2;
servoW = 12.4;
cutR = servoW*.30;

nut_w_flats=4.95;
nut_th = 2;
nut_trap_r = nut_w_flats/2/cos(180/6) +.01;



difference(){
  union(){
    translate([-bth,(hY-5.5),0])
      cube([hZ,10,hZ]);

    translate([3,-(yY-ycY)+5,0])
      cube([hZ,5,hZ]);

    batteryClamp();

    translate([-yX+bth/2,-(yY-ycY)/2,0])
      cube([yX,yY,yZ]);

  }

  //servo wire cutouts
    translate([-6.5,ycY-.3,yZ/2])
      cube([1.5,6,25]);

    translate([-5.8,ycY-.3,yZ/2-.2])
      rotate([270,0,0])
      cylinder(d=3.5,h=15,$fn=30);

  //screw hole servo
    translate([-yX-.1,ycY+(yY-ycY)/4,8.6])
      rotate([0,90,0])
      cylinder(d=screwDia,h=10,$fn=30);

  //screw hole servo
  translate([-yX-.1,-(yY-ycY)/4,8.6])
    rotate([0,90,0])
    cylinder(d=screwDia,h=10,$fn=30);
    
  //clamp cutout
  translate([-10,27.6,-.1])
    cube([25,1.5,25]);
      
  //clamp screw hole
  translate([-bth/2+.5,ycY-(yY-ycY)/4,hZ/2])
    rotate([270,0,0])
    cylinder(d=screwDiaClamp,h=16,$fn=30);
      
  //nut trap
  color("purple")
    translate([-nut_w_flats/2+1,.2+ ycY+(yY-ycY)/4,nut_w_flats/2])
    rotate([90,30,0])
    cylinder(h=nut_th*1.25,r1=nut_trap_r, r2=nut_trap_r,$fn=6);

    translate([-nut_w_flats/2+1,.2+ ycY+(yY-ycY)/4,nut_w_flats/4])
      rotate([90,30,0])
      cylinder(h=nut_th*1.25,r1=nut_trap_r, r2=nut_trap_r,$fn=6);      


  //nut trap
  color("purple")
    translate([-2.5,-(yY-ycY)/4,nut_w_flats/2])
    rotate([0,90,0])
    cylinder(h=nut_th*1.15,r1=nut_trap_r, r2=nut_trap_r,$fn=6);
      
  translate([-2.5,-(yY-ycY)/4-1.7,nut_w_flats/2])
    rotate([0,90,0])
    cylinder(h=nut_th*1.15,r1=nut_trap_r, r2=nut_trap_r,$fn=6);
   
  //clamp screw hole
   translate([-2.5,-(yY-ycY)/4,nut_w_flats/2])
    rotate([0,90,0])
    cylinder(d=screwDiaClamp,h=35,$fn=30); 
      
        
  //cutout
  translate([-yX+bth/2-1.5,0,1])
  cube([yX,ycY,yZ+.1]);
}

module batteryClamp() {
difference(){
  //outside battery edge
 color ("blue") 
  cube([hX+bth+tol,hY+bth+tol,hZ]);
 
  //battery hole
  color ("red") 
    translate([bth/2,bth/2,0])
    cube([hX+tol,hY+tol,hZ]);
}}