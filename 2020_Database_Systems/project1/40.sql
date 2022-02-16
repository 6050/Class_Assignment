SELECT hometown AS '트레이너의 출신 도시', nickname AS '가장 레벨이 높은 포켓몬의 별명'
FROM Trainer AS T1, CatchedPokemon AS C
WHERE T1.id = owner_id AND level >= ALL(SELECT level
                                        FROM Trainer AS T2, CatchedPokemon
                                        WHERE T2.id = owner_id AND T1.hometown = T2.hometown)
ORDER BY hometown;