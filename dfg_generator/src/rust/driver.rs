extern "C" {
    fn rust_entry() -> i32;
}

fn main() {
    unsafe {
        let rc = rust_entry();
        std::process::exit(rc);
    }
}