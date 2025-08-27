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
    let args = misc::args();
    let exercise = args[1].as_str();
    let result = match exercise {
        "1a" => exercise_1::a(),
        "1b" => exercise_1::b(),
        "2a" => exercise_2::a(),
        "2b" => exercise_2::b(),
        "3a" => exercise_3::a(),
        "3b" => exercise_3::b(),
        "4a" => exercise_4::a(),
        "4b" => exercise_4::b(),
        "5a" => exercise_5::a(),
        "5b" => exercise_5::b(),
        "6a" => exercise_6::a(),
        "6b" => exercise_6::b(),
        "7a" => exercise_7::a(),
        "7b" => exercise_7::b(),
        "8a" => exercise_8::a(),
        "8b" => exercise_8::b(),
        "9a" => exercise_9::a(),
        "9b" => exercise_9::b(),
        "10a" => exercise_10::a(),
        "10b" => exercise_10::b(),
        "11a" => exercise_11::a(),
        "11b" => exercise_11::b(),
        "12a" => exercise_12::a(),
        "12b" => exercise_12::b(),
        "13a" => exercise_13::a(),
        "13b" => exercise_13::b(),
        "14a" => exercise_14::a(),
        "14b" => exercise_14::b(),
        "15a" => exercise_15::a(),
        "15b" => exercise_15::b(),
        "16a" => exercise_16::a(),
        "16b" => exercise_16::b(),
        "17a" => exercise_17::a(),
        "17b" => exercise_17::b(),
        "18a" => exercise_18::a(),
        "18b" => exercise_18::b(),
        "19a" => exercise_19::a(),
        "19b" => exercise_19::b(),
        "20a" => exercise_20::a(),
        "20b" => exercise_20::b(),
        "21a" => exercise_21::a(),
        "21b" => exercise_21::b(),
        "22a" => exercise_22::a(),
        "22b" => exercise_22::b(),
        "23a" => exercise_23::a(),
        "23b" => exercise_23::b(),
        "24a" => exercise_24::a(),
        "24b" => exercise_24::b(),
        "25a" => exercise_25::a(),
        "25b" => exercise_25::b(),
        _ => panic!(),
    };

    println!("{}", result);
}
