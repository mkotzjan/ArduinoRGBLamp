$fn = 30;


outerSize = [47, 110, 17];
innerSize = [43, 106, 17];
difference(){
union(){
// Top
translate([0, 0, 0]){
    difference(){
        cube(outerSize);
        translate([2, 2, 2]){
            cube(innerSize);
        }
        translate([1,1,15]){
                cube([45.2, 108.2, 2]);
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
translate([47, 250, 0]){
    rotate(180,0,0){
        difference(){
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
                        cube([44.8, 107.8, 2]);
                    }
                    translate([2,2,17]){
                        cube([43, 106, 2]);
                    }
                }
            }
            translate([33, 0, 13]){
                cube([6, 2,8]);
            }
            translate([20, 108, 17]){
                cube([7,2,4]);
            }
        }
    }
}

translate([8.5,140,10]){
    rotate([90,0,0]){
    cylinder(r=1,5, h=30);
    }
    translate([30,0,0]){
        rotate([90,0,0]){
            cylinder(r=1,5, h=30);
        }
    }
}

translate([5.5,115,9]){
    cube([37,20,2]);
    translate([0, 12, 2]){
        linear_extrude(height = 2) {
            text("Arduino", font = "Arial Black", size = 7);
        }
    }
    translate([6, 7, 2]){
        linear_extrude(height = 2) {
            text("-", font = "Arial Black", size = 7);
        }
    }
    translate([0, 0, 2]){
        linear_extrude(height = 2) {
            text("RGB.stl", font = "Arial Black", size = 7);
        }
    }
}
}
translate([46.5, 20, 12]){
    rotate([90,180,90]){
        linear_extrude(height = 0.5) {
            text(" ♥", font = "Arial Black", size = 10);
        }
    }
}
}
/*
translate([60,0, 0]){
    difference(){
        hull(){
            translate([1, 1, 0]){
                cylinder(r=1, h=17);
            }
            translate([46, 1, 0]){
                cylinder(r=1, h=17);
            }
            translate([1, 109, 0]){
                cylinder(r=1, h=17);
            }
            translate([46, 109, 0]){
                cylinder(r=1, h=17);
            }
        }
        translate([2,2,2]){
            hull(){
                translate([0, 0, 0]){
                    cylinder(r=1, h=17);
                }
                translate([43, 0, 0]){
                    cylinder(r=1, h=17);
                }
                translate([0, 106, 0]){
                    cylinder(r=1, h=17);
                }
                translate([43, 106, 0]){
                    cylinder(r=1, h=17);
                }
            }
        }
        translate([1.5,1.5,15]){
            hull(){
                translate([0, 0, 0]){
                    cylinder(r=1, h=17);
                }
                translate([44, 0, 0]){
                    cylinder(r=1, h=17);
                }
                translate([0, 107, 0]){
                    cylinder(r=1, h=17);
                }
                translate([44, 107, 0]){
                    cylinder(r=1, h=17);
                }
            }
        }
    }
}
*/