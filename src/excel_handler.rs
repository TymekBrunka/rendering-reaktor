use rfd::FileDialog;
use calamine::{Reader, open_workbook, Xlsx, DataType, Data, ExcelDateTime};

use crate::types::Row;
#[derive(Default)]
struct ColumnMapping {
	rb1ko_po4: usize,
	rb1ko_nh4: usize,
	rb2ko_po4: usize,
	rb2ko_nh4: usize,
}

pub fn click_action() -> Vec<Row> {
	let mut colstart = 0;
	let mut colend = 0;
	let mut rowstart = 0;
	let mut colmap: ColumnMapping = Default::default();
	let mut found_header_row = false;

	let mut rows: Vec<Row> = Vec::new();

	let file = FileDialog::new()
        .add_filter("excel", &["xlsx", "xls"])
        .set_directory("/")
        .pick_file();

    if file.is_some() {
        let file = file.unwrap();
        let filepath = file.as_path().to_str().unwrap();
        let mut workbook: Xlsx<_> = open_workbook(filepath).expect("Nie można otworzyć pliku.");
        if let Ok(r) = workbook.worksheet_range("Arkusz1") {

        	//finding header row
        	let mut it: Data;
        	let mut iter = r.rows().enumerate().into_iter();
            while let Some((i, row)) = iter.next() {
                if row.len() > 0 {
                	// println!("row={:?}, row[0]={:?}", row, row[0]);
                	//checking if it has DataCzas column
                	for (j, column) in row.iter().enumerate() {
                		match column {
                			Data::String(name) => {
                				match name.as_str() {
                					"DataCzas" => {
		                				// println!("i={}, j={}", i, j);
		                				rowstart = i;
		                				colstart = j;
		                				found_header_row = true;
		                			}
		                			"I21_RB1KO_PO4.Wartosc" => {colmap.rb1ko_po4 = j;}
		                			"I21_RB1KO_NH4.Wartosc" => {colmap.rb1ko_nh4 = j;}
		                			"I21_RB2KO_PO4.Wartosc" => {colmap.rb2ko_po4 = j;}
		                			"I21_RB2KO_NH4.Wartosc" => {colmap.rb2ko_nh4 = j;}
		                			"wwRetrievalMode" => {colend = j;}
		                			_ => {}
		                		}
                			},
                			_ => {}
                		}
                	}
                }

                if found_header_row { break; }
            }

            let mut row_to_be_added: Row = Default::default();

            while let Some((_, row)) = iter.next() {
            	//
            	// filling row data
            	//

            	if let Data::DateTime(dataczas) = row[colstart] {
            		if row.len() < colend { continue; }
            		
            		row_to_be_added = Row {
            			dataczas: row_to_be_added.dataczas,
            			..Default::default()
            		};
            		row_to_be_added.dataczas = dataczas.as_datetime().unwrap().to_string();

            		rows.push(row_to_be_added.clone());

            		// println!("{:?}", row);
            	}

            	if let Data::Float(rb1kopo4) = row[colmap.rb1ko_po4] {
            		row_to_be_added.rb1ko_po4 = rb1kopo4;
            		let len = rows.len();
            		rows[len - 1].rb1kopo4s = format!("{:.2} mg/l", rb1kopo4);
            	}

            	if let Data::Float(rb1konh4) = row[colmap.rb1ko_nh4] {
            		row_to_be_added.rb1ko_nh4 = rb1konh4;
            		let len = rows.len();
            		rows[len - 1].rb1konh4s = format!("{:.2} mg/l", rb1konh4);
            	}

            	if let Data::Float(rb2kopo4) = row[colmap.rb1ko_po4] {
            		row_to_be_added.rb1ko_po4 = rb2kopo4;
            		let len = rows.len();
            		rows[len - 1].rb2kopo4s = format!("{:.2} mg/l", rb2kopo4);
            	}

            	if let Data::Float(rb2konh4) = row[colmap.rb2ko_nh4] {
            		row_to_be_added.rb2ko_nh4 = rb2konh4;
            		let len = rows.len();
            		rows[len - 1].rb2konh4s = format!("{:.2} mg/l", rb2konh4);
            	}
            }
        }
    }

    rows
}