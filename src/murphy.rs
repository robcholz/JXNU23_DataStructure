pub(crate) fn mur_profiler_module(name: &str) {
    println!("\x1b[1m\x1b[33m-----{}-----\x1b[0m", name);
}

pub(crate) fn mur_profiler_test_lambda(name: &str, status: bool, func: &dyn Fn()) {
    let result = if status {
        "\x1b[32mSuccess\x1b[0m"
    } else {
        "\x1b[31mUnsuccess\x1b[0m"
    };
    println!("[{}] [\x1b[36m{}\x1b[0m]", result, name);
    func();
}
