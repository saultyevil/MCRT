/*
 * Main file of Rust implementation
 */

// extern crate time;
use time::PreciseTime;
use rand::Rng;
use std::f64;

struct PhotonPacket{
    x: f64,
    y: f64,
    z: f64,
    cos_theta: f64,
    sin_theta: f64,
    cos_phi: f64,
    sin_phi: f64,
    escaped: bool
}

fn random_number() -> f64 {
    return rand::thread_rng().gen_range(0.0, 1.0);
}

fn emit_photon() -> PhotonPacket {
    let cos_theta = f64::sqrt(random_number());
    let sin_theta = f64::sqrt(1.0 - cos_theta.powf(2.0));
    let cos_phi = f64::cos(2.0 * std::f64::consts::PI * random_number());
    let sin_phi = f64::sqrt(1.0 - cos_phi.powf(2.0));
    let photon = PhotonPacket{
        x: 0.0, y: 0.0, z: 0.0, cos_theta: cos_theta, sin_theta: sin_theta,
        cos_phi: cos_phi, sin_phi: sin_phi, escaped: false
    };

    return photon;
}

fn scatter_photon(photon: &mut PhotonPacket) {
    photon.cos_theta = f64::sqrt(random_number());
    photon.sin_theta = f64::sqrt(1.0 - photon.cos_theta.powf(2.0));
    photon.cos_phi = f64::cos(2.0 * std::f64::consts::PI * random_number());
    photon.sin_phi = f64::sqrt(1.0 - photon.cos_phi.powf(2.0));
}

fn move_photon(photon: &mut PhotonPacket, length: f64) {
    photon.x += length * photon.sin_theta * photon.cos_phi;
    photon.y += length * photon.sin_theta * photon.sin_phi;
    photon.z += length * photon.cos_theta;
}

fn transport_photons(n_photons: i32, progress: i32, tau_max: f64, scat_albedo: f64) {
    for n in 1..n_photons + 1 {
        let mut photon = emit_photon();

        while photon.escaped == false {
            let tau_scat = -1.0 * f64::ln(random_number());
            let dist_move = tau_max / tau_scat;
            move_photon(&mut photon, dist_move);

            if photon.z < 0.0 {
                photon = emit_photon();
            }
            else if photon.z > 1.0 {
                photon.escaped = true;
            }
            else {
                if random_number() < scat_albedo {
                    scatter_photon(&mut photon);
                }
            }
        }

        if n % progress == 0 {
            println!{"{}% of photons transported", (n as f64) / (n_photons as f64) * 100.0};
        }
    }
}


fn main() {
    println!("Beginning simulation");
    let start_time = PreciseTime::now();

    /*
     * Intialize parameters which control the simulation
     */

    let n_photons : i32 = 1e5 as i32;
    let progress : i32 = n_photons / 10;
    let scat_albedo : f64 = 1.0;
    let tau_max: f64 = 7.0;

    transport_photons(n_photons, progress, tau_max, scat_albedo);

    let end_time = PreciseTime::now();
    println!("Simulation completed in {} seconds", start_time.to(end_time));
}
