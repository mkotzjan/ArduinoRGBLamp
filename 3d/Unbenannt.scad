$fn = 30;

outerSize = [47, 110, 17];
innerSize = [43, 106, 17];

// Top
translate([4, 0, 0]){
    difference(){
        cube(outerSize);
        translate([2, 2, 2]){
            cube(innerSize);
        }
        translate([1,1,15]){
                cube([45, 108, 2]);
        }
        translate([23.5, 110, 17]){
            rotate([90, 0, 0]){
                cylinder(d=7, h=2);
            }
        }
        translate([8, 0, 13]){
            cube([6, 2,4]);
        }
    }
}

// Bottom
translate([-53, 0, 0]){
    union(){
        difference(){
            cube(outerSize);
            translate([2, 2, 2]){
                cube(innerSize);
            }
            translate([23.5, 110, 17]){
                rotate([90, 0, 0]){
                    cylinder(d=7, h=2);
                }
            }
            translate([33, 0, 13]){
                cube([6, 2,8]);
            }
        }
        difference(){
            translate([1,1,17]){
                cube([45, 108, 2]);
            }
            translate([2,2,17]){
                cube([43, 106, 2]);
            }
        }
    }
}
/*
        translate([1,1,15]){
                cube([45, 108, 2]);
        }
*/