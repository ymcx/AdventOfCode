use std::time::Instant;

mod exercise_1;
mod exercise_10;
mod exercise_11;
mod exercise_12;
mod exercise_13;
mod exercise_14;
mod exercise_15;
mod exercise_16;
mod exercise_17;
mod exercise_18;
mod exercise_19;
mod exercise_2;
mod exercise_20;
mod exercise_21;
mod exercise_22;
mod exercise_23;
mod exercise_24;
mod exercise_25;
mod exercise_3;
mod exercise_4;
mod exercise_5;
mod exercise_6;
mod exercise_7;
mod exercise_8;
mod exercise_9;
mod misc;

fn main() {
    let time = Instant::now();
    let args = misc::args();
    let exercise = args[1].as_str();
    let input = args[2].as_str();
    let result = match exercise {
        "1a" => exercise_1::a(input),
        "1b" => exercise_1::b(input),
        "2a" => exercise_2::a(input),
        "2b" => exercise_2::b(input),
        "3a" => exercise_3::a(input),
        "3b" => exercise_3::b(input),
        "4a" => exercise_4::a(input),
        "4b" => exercise_4::b(input),
        "5a" => exercise_5::a(input),
        "5b" => exercise_5::b(input),
        "6a" => exercise_6::a(input),
        "6b" => exercise_6::b(input),
        "7a" => exercise_7::a(input),
        "7b" => exercise_7::b(input),
        "8a" => exercise_8::a(input),
        "8b" => exercise_8::b(input),
        "9a" => exercise_9::a(input),
        "9b" => exercise_9::b(input),
        "10a" => exercise_10::a(input),
        "10b" => exercise_10::b(input),
        "11a" => exercise_11::a(input),
        "11b" => exercise_11::b(input),
        "12a" => exercise_12::a(input),
        "12b" => exercise_12::b(input),
        "13a" => exercise_13::a(input),
        "13b" => exercise_13::b(input),
        "14a" => exercise_14::a(input),
        "14b" => exercise_14::b(input),
        "15a" => exercise_15::a(input),
        "15b" => exercise_15::b(input),
        "16a" => exercise_16::a(input),
        "16b" => exercise_16::b(input),
        "17a" => exercise_17::a(input),
        "17b" => exercise_17::b(input),
        "18a" => exercise_18::a(input),
        "18b" => exercise_18::b(input),
        "19a" => exercise_19::a(input),
        "19b" => exercise_19::b(input),
        "20a" => exercise_20::a(input),
        "20b" => exercise_20::b(input),
        "21a" => exercise_21::a(input),
        "21b" => exercise_21::b(input),
        "22a" => exercise_22::a(input),
        "22b" => exercise_22::b(input),
        "23a" => exercise_23::a(input),
        "23b" => exercise_23::b(input),
        "24a" => exercise_24::a(input),
        "24b" => exercise_24::b(input),
        "25a" => exercise_25::a(input),
        "25b" => exercise_25::b(input),
        _ => panic!(),
    };

    println!("{}\n{:?}", result, time.elapsed());
}
