//! A simple 1D Monte Carlo Radiative Transfer simulation

use rand::Rng;
use std::f64;
use std::i32;
use time::PreciseTime;
use libm;

/// Structure to contain photon variables
struct PhotonPacket {
    x: f64,
    y: f64,
    z: f64,
    cos_theta: f64,
    sin_theta: f64,
    cos_phi: f64,
    sin_phi: f64,
    weight: f64,
    escaped: bool,
}

/// Generates a uniform random number inclusive range [0.0 1.0].
fn random_number() -> f64 {
    return rand::thread_rng().gen_range(0.0, 1.0);
}

/// Creates a photon at the origin of the system with a random isotropic direction
fn photon_create_at_origin() -> PhotonPacket {
    let cos_theta = f64::sqrt(random_number());
    let sin_theta = f64::sqrt(1.0 - cos_theta.powf(2.0));
    let cos_phi = f64::cos(2.0 * f64::consts::PI * random_number());
    let sin_phi = f64::sqrt(1.0 - cos_phi.powf(2.0));
    let photon = PhotonPacket {
        x: 0.0,
        y: 0.0,
        z: 0.0,
        cos_theta: cos_theta,
        sin_theta: sin_theta,
        cos_phi: cos_phi,
        sin_phi: sin_phi,
        weight: 1.0,
        escaped: false,
    };

    return photon;
}

/// Isotropically scatter a photon, i.e. give it a new direction
fn photon_isotropic_scatter(photon: &mut PhotonPacket) {
    photon.cos_theta = f64::sqrt(random_number());
    photon.sin_theta = f64::sqrt(1.0 - photon.cos_theta.powf(2.0));
    photon.cos_phi = f64::cos(2.0 * f64::consts::PI * random_number());
    photon.sin_phi = f64::sqrt(1.0 - photon.cos_phi.powf(2.0));
}

/// Move a photon by the given distance.
fn photon_move_length(photon: &mut PhotonPacket, length: f64) {
    photon.x += length * photon.sin_theta * photon.cos_phi;
    photon.y += length * photon.sin_theta * photon.sin_phi;
    photon.z += length * photon.cos_theta;
}

/// Transport all photon packets through the medium
fn transport_all_photons(n_photons: i32, progress: i32, tau_max: f64, scat_albedo: f64) {

    // Initialize the histogram for binning

    let n_bins = 45;
    let d_theta = 1.0 / n_bins as f64;
    let half_width = 0.5 * d_theta;

    let mut angle_bins = vec![0.0; n_bins];
    let mut weight_bins = vec![0.0; n_bins];
    let mut intensity = vec![0.0; n_bins];

    for n in 0..n_bins {
        angle_bins[n] = libm::acos(n as f64 * d_theta + half_width)
    }

    // Transport all the photons

    for n in 1..n_photons + 1 {
        let mut photon = photon_create_at_origin();

        while photon.escaped == false {
            let tau_scat = -1.0 * f64::ln(random_number());
            let length = tau_scat / tau_max;
            photon_move_length(&mut photon, length);

            if photon.z < 0.0 {
                photon = photon_create_at_origin();
            } else if photon.z > 1.0 {
                photon.escaped = true;
            } else {
                if random_number() < scat_albedo {
                    photon_isotropic_scatter(&mut photon);
                }
            }
        }

        // Bin the photon's escape direction into the appropriate angle bin

        let m = i32::abs((photon.cos_theta * n_bins as f64) as i32) as usize;
        weight_bins[m] += photon.weight;

        // Track progress

        if n % progress == 0 {
            println! {"{}% of photons transported", (n as f64) / (n_photons as f64) * 100.0};
        }
    }

    // Calculate the intensity from the weight and angles

    for n in 0..n_bins {
        intensity[n] = weight_bins[n] * n_bins as f64 / (2.0 * n_photons as f64 * f64::cos(angle_bins[n]));
    }
}

/// Main function of the program
fn main() {
    println!("Beginning simulation\n");
    let start_time = PreciseTime::now();
    let n_photons: i32 = 1e5 as i32;
    let progress: i32 = n_photons / 10;
    let scat_albedo: f64 = 1.0;
    let tau_max: f64 = 7.0;

    println!("Using the following parameter values:");
    println!("n_photons    {}", n_photons);
    println!("progress     {}", progress);
    println!("scat_albedo  {}", scat_albedo);
    println!("tau_max      {}", tau_max);

    println!("\nBeginning photon transport\n");
    transport_all_photons(n_photons, progress, tau_max, scat_albedo);

    let end_time = PreciseTime::now();
    println!(
        "\nSimulation completed in {} seconds",
        start_time.to(end_time),
    );
}
