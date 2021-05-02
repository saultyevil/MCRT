/*
 * Main file of Rust implementation
 */

// extern crate time;
use time::PreciseTime;
use rand::distributions::{Distribution, Uniform};

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


fn emit_photon() -> PhotonPacket {

    let between = Uniform::from(0.0..1.0);
    let mut rng = rand::thread_rng();

    let xi1: f64 = between.sample(&mut rng);
    let xi2: f64 = between.sample(&mut rng);

    let cos_theta = xi1.sqrt();
    let sin_theta = (1.0 - cos_theta.powf(2.0)).sqrt();
    let cos_phi = (2.0 * std::f64::consts::PI * xi2).cos();
    let sin_phi = (1.0 - cos_phi.powf(2.0)).sqrt();

    let photon = PhotonPacket{
        x: 0.0, y: 0.0, z: 0.0, cos_theta: cos_theta, sin_theta: sin_theta,
        cos_phi: cos_phi, sin_phi: sin_phi, escaped: false
    };

    return photon;
}

fn scatter_photon(photon: &mut PhotonPacket) {

    let between = Uniform::from(0.0..1.0);
    let mut rng = rand::thread_rng();
    let xi1: f64 = between.sample(&mut rng);
    let xi2: f64 = between.sample(&mut rng);

    photon.cos_theta = xi1.sqrt();
    photon.sin_theta = (1.0 - photon.cos_theta.powf(2.0)).sqrt();
    photon.cos_phi = (2.0 * std::f64::consts::PI * xi2).cos();
    photon.sin_phi = (1.0 - photon.cos_phi.powf(2.0)).sqrt();
}



fn transport_photons(n_photons: i32, progress: i32, tau_max: f64, scat_albedo: f64) {

    let between = Uniform::from(0.0..1.0);
    let mut rng = rand::thread_rng();


    for n in 0..n_photons {
        let mut photon = emit_photon();
        let mut tau = 0.0;

        while tau < tau_max {

            let xi = between.sample(&mut rng);
            if xi < scat_albedo {
                scatter_photon(&mut photon);
            }

            tau += 0.5;
        }

        if n % progress == 0 {
            println!{"{}% of photons transported", (n as f64) / (n_photons as f64) * 100.0};
        }
    }
}


fn main() {

    let start_time = PreciseTime::now();

    /*
     * Intialize parameters which control the simulation
     */

    let n_photons : i32 = 1e6 as i32;
    let progress : i32 = n_photons / 10;
    let scat_albedo : f64 = 0.5;
    let tau_max: f64 = 7.0;

    transport_photons(n_photons, progress, tau_max, scat_albedo);

    let end_time = PreciseTime::now();
    println!("Simulation completed in {} seconds", start_time.to(end_time));
}
