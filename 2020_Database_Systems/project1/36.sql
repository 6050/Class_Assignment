SELECT T.name AS '최종 진화 형태의 포켓몬을 잡은 트레이너의 이름'
FROM Trainer AS T, CatchedPokemon, Pokemon AS P, Evolution
WHERE T.id = owner_id AND pid = P.id AND P.id = after_id AND after_id NOT IN (SELECT before_id FROM Evolution)
ORDER BY T.name;