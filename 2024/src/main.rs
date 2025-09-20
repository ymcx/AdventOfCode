mod day01;
mod day02;
mod day03;
mod day04;
mod day05;
mod day06;
mod day07;
mod day08;
mod day09;
mod day10;
mod day11;
mod day12;
mod day13;
mod day14;
mod day15;
mod day16;
mod day17;
mod day18;
mod day19;
mod day20;
mod day21;
mod day22;
mod day23;
mod day24;
mod day25;
mod misc;

fn main() {
    let args = misc::args();
    let exercise = args[1].as_str();
    let input = args[2].as_str();
    let result = match exercise {
        "01A" => day01::a(input),
        "01B" => day01::b(input),
        "02A" => day02::a(input),
        "02B" => day02::b(input),
        "03A" => day03::a(input),
        "03B" => day03::b(input),
        "04A" => day04::a(input),
        "04B" => day04::b(input),
        "05A" => day05::a(input),
        "05B" => day05::b(input),
        "06A" => day06::a(input),
        "06B" => day06::b(input),
        "07A" => day07::a(input),
        "07B" => day07::b(input),
        "08A" => day08::a(input),
        "08B" => day08::b(input),
        "09A" => day09::a(input),
        "09B" => day09::b(input),
        "10A" => day10::a(input),
        "10B" => day10::b(input),
        "11A" => day11::a(input),
        "11B" => day11::b(input),
        "12A" => day12::a(input),
        "12B" => day12::b(input),
        "13A" => day13::a(input),
        "13B" => day13::b(input),
        "14A" => day14::a(input),
        "14B" => day14::b(input),
        "15A" => day15::a(input),
        "15B" => day15::b(input),
        "16A" => day16::a(input),
        "16B" => day16::b(input),
        "17A" => day17::a(input),
        "17B" => day17::b(input),
        "18A" => day18::a(input),
        "18B" => day18::b(input),
        "19A" => day19::a(input),
        "19B" => day19::b(input),
        "20A" => day20::a(input),
        "20B" => day20::b(input),
        "21A" => day21::a(input),
        "21B" => day21::b(input),
        "22A" => day22::a(input),
        "22B" => day22::b(input),
        "23A" => day23::a(input),
        "23B" => day23::b(input),
        "24A" => day24::a(input),
        "24B" => day24::b(input),
        "25A" => day25::a(input),
        _ => return,
    };

    println!("{}", result);
}

#[test]
fn test() {
    assert!(day01::a("input/01") == "2580760");
    assert!(day01::b("input/01") == "25358365");
    assert!(day02::a("input/02") == "549");
    assert!(day02::b("input/02") == "589");
    assert!(day03::a("input/03") == "162813399");
    assert!(day03::b("input/03") == "53783319");
    assert!(day04::a("input/04") == "2554");
    assert!(day04::b("input/04") == "1916");
    assert!(day05::a("input/05") == "5391");
    assert!(day05::b("input/05") == "6142");
    assert!(day06::a("input/06") == "5067");
    assert!(day06::b("input/06") == "1793");
    assert!(day07::a("input/07") == "7710205485870");
    assert!(day07::b("input/07") == "20928985450275");
    assert!(day08::a("input/08") == "423");
    assert!(day08::b("input/08") == "1287");
    assert!(day09::a("input/09") == "6378826667552");
    assert!(day09::b("input/09") == "6413328569890");
    assert!(day10::a("input/10") == "733");
    assert!(day10::b("input/10") == "1514");
    assert!(day11::a("input/11") == "209412");
    assert!(day11::b("input/11") == "248967696501656");
    assert!(day12::a("input/12") == "1415378");
    assert!(day12::b("input/12") == "862714");
    assert!(day13::a("input/13") == "29201");
    assert!(day13::b("input/13") == "104140871044942");
    assert!(day14::a("input/14") == "224554908");
    assert!(day14::b("input/14") == "6644");
    assert!(day15::a("input/15") == "1430439");
    assert!(day15::b("input/15") == "1458740");
    assert!(day16::a("input/16") == "101492");
    assert!(day16::b("input/16") == "543");
    assert!(day17::a("input/17") == "7,3,5,7,5,7,4,3,0");
    assert!(day17::b("input/17") == "105734774294938");
    assert!(day18::a("input/18") == "280");
    assert!(day18::b("input/18") == "28,56");
    assert!(day19::a("input/19") == "365");
    assert!(day19::b("input/19") == "730121486795169");
    assert!(day20::a("input/20") == "1327");
    assert!(day20::b("input/20") == "985737");
    assert!(day21::a("input/21") == "174124");
    assert!(day21::b("input/21") == "216668579770346");
    assert!(day22::a("input/22") == "20071921341");
    assert!(day22::b("input/22") == "2242");
    assert!(day23::a("input/23") == "1149");
    assert!(day23::b("input/23") == "as,co,do,kh,km,mc,np,nt,un,uq,wc,wz,yo");
    assert!(day24::a("input/24") == "59364044286798");
    assert!(day24::b("input/24") == "cbj,cfk,dmn,gmt,qjj,z07,z18,z35");
    assert!(day25::a("input/25") == "3196");
}
