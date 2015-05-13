#include "cpplinq.hpp"

namespace cpplinq
{               
	namespace detail
	{

		// -------------------------------------------------------------------------

        template <typename TAccumulator>
        struct aggregate_without_seed_builder : base_builder
        {
            typedef                 aggregate_without_seed_builder<TAccumulator>    this_type       ;
            typedef                 TAccumulator                                    accumulator_type;

            accumulator_type        accumulator;

            CPPLINQ_INLINEMETHOD aggregate_without_seed_builder (accumulator_type accumulator) throw ()
                :   accumulator (std::move (accumulator))
            {
            }

            CPPLINQ_INLINEMETHOD aggregate_without_seed_builder (aggregate_without_seed_builder const & v) throw ()
                :   accumulator (v.accumulator)
            {
            }

            CPPLINQ_INLINEMETHOD aggregate_without_seed_builder (aggregate_without_seed_builder && v) throw ()
                :   accumulator (std::move (v.accumulator))
            {
            }

            template<typename TRange>
            CPPLINQ_INLINEMETHOD auto build (TRange range) -> decltype (accumulator (range.front (), range.front ()))
            {
				if (!range.next ())
				{
					assert (false);
					throw programming_error_exception ();
				}

                auto sum = range.front ();
                while (range.next ())
                {
                    sum = accumulator (sum, range.front ());
                }
                return std::move (sum);
            }

        };

		// -------------------------------------------------------------------------

		template<typename TKeyPredicate>
		struct groupby_builder : base_builder
		{
			typedef                     groupby_builder<TKeyPredicate>    this_type           ;
			typedef                     TKeyPredicate                       key_predicate_type  ;

			key_predicate_type          key_predicate   ;

			CPPLINQ_INLINEMETHOD explicit groupby_builder (key_predicate_type key_predicate) throw ()
				:   key_predicate   (std::move (key_predicate))
			{
			}

			CPPLINQ_INLINEMETHOD groupby_builder (groupby_builder const & v)
				:   key_predicate (v.key_predicate)
			{
			}

			CPPLINQ_INLINEMETHOD groupby_builder (groupby_builder && v) throw ()
				:   key_predicate (std::move (v.key_predicate))
			{
			}

			template<typename TRange>
			CPPLINQ_INLINEMETHOD from_copy_range<typename cleanup_type<
				typename std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename TRange::value_type>
				>
			>::type> build (TRange range)
			{
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename TRange::value_type>
				> groupings;

				while (range.next ())
				{
					auto value  = range.front ();
					auto key    = key_predicate (value);
					groupings[key].push_back (value);
				}

				return from_copy (std::move (groupings));
			}
		};

		// -------------------------------------------------------------------------

		template<typename TKeyPredicate, typename TSelector>
		struct groupby_selector_builder : base_builder
		{
			typedef		groupby_selector_builder<TKeyPredicate, TSelector>    this_type           ;
			typedef     TKeyPredicate                       key_predicate_type  ;
			typedef     TSelector							selector_type		;

			key_predicate_type          key_predicate   ;
			selector_type				selector   ;

			CPPLINQ_INLINEMETHOD explicit groupby_selector_builder (
				key_predicate_type key_predicate
				, selector_type selector) throw ()
				:   key_predicate   (std::move (key_predicate))
				,   selector		(std::move (selector))
			{
			}

			CPPLINQ_INLINEMETHOD groupby_selector_builder (groupby_selector_builder const & v)
				:   key_predicate	(v.key_predicate)
				,   selector		(v.selector)
			{
			}

			CPPLINQ_INLINEMETHOD groupby_selector_builder (groupby_selector_builder && v) throw ()
				:   key_predicate	(std::move (v.key_predicate))
				,   selector		(std::move (v.selector))
			{
			}

			template<typename TRange>
			CPPLINQ_INLINEMETHOD from_copy_range<typename cleanup_type<
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename get_transformed_type<selector_type, typename TRange::value_type>::type>
				>
			>::type> build (TRange range)
			{
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename get_transformed_type<selector_type, typename TRange::value_type>::type>
				> groupings;

				while (range.next ())
				{
					auto value  = range.front ();
					auto key    = key_predicate (value);
					groupings[key].push_back (selector (value));
				}

				return from_copy (std::move (groupings));
			}
		};

		// -------------------------------------------------------------------------

		template<typename TKeyPredicate>
		struct to_groups_builder : base_builder
		{
			typedef                     to_groups_builder<TKeyPredicate>    this_type           ;
			typedef                     TKeyPredicate                       key_predicate_type  ;

			key_predicate_type          key_predicate   ;

			CPPLINQ_INLINEMETHOD explicit to_groups_builder (key_predicate_type key_predicate) throw ()
				:   key_predicate   (std::move (key_predicate))
			{
			}

			CPPLINQ_INLINEMETHOD to_groups_builder (to_groups_builder const & v)
				:   key_predicate (v.key_predicate)
			{
			}

			CPPLINQ_INLINEMETHOD to_groups_builder (to_groups_builder && v) throw ()
				:   key_predicate (std::move (v.key_predicate))
			{
			}

			template<typename TRange>
			CPPLINQ_INLINEMETHOD
				typename std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename TRange::value_type>
				> build (TRange range)
			{
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename TRange::value_type>
				> groupings;

				while (range.next ())
				{
					auto value  = range.front ();
					auto key    = key_predicate (value);
					groupings[key].push_back (value);
				}

				return std::move (groupings);
			}
		};

		// -------------------------------------------------------------------------

		template<typename TKeyPredicate, typename TSelector>
		struct to_groups_selector_builder : base_builder
		{
			typedef		to_groups_selector_builder<TKeyPredicate, TSelector>    this_type           ;
			typedef     TKeyPredicate                       key_predicate_type  ;
			typedef     TSelector							selector_type		;

			key_predicate_type          key_predicate   ;
			selector_type				selector   ;

			CPPLINQ_INLINEMETHOD explicit to_groups_selector_builder (
				key_predicate_type key_predicate
				, selector_type selector) throw ()
				:   key_predicate   (std::move (key_predicate))
				,   selector		(std::move (selector))
			{
			}

			CPPLINQ_INLINEMETHOD to_groups_selector_builder (to_groups_selector_builder const & v)
				:   key_predicate	(v.key_predicate)
				,   selector		(v.selector)
			{
			}

			CPPLINQ_INLINEMETHOD to_groups_selector_builder (to_groups_selector_builder && v) throw ()
				:   key_predicate	(std::move (v.key_predicate))
				,   selector		(std::move (v.selector))
			{
			}

			template<typename TRange>
			CPPLINQ_INLINEMETHOD 
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename get_transformed_type<selector_type, typename TRange::value_type>::type>
				> build (TRange range)
			{
				std::map<
					typename get_transformed_type<key_predicate_type, typename TRange::value_type>::type
					, std::vector<typename get_transformed_type<selector_type, typename TRange::value_type>::type>
				> groupings;

				while (range.next ())
				{
					auto value  = range.front ();
					auto key    = key_predicate (value);
					groupings[key].push_back (selector (value));
				}

				return std::move (groupings);
			}
		};
	}

	template <typename TAccumulator>
	CPPLINQ_INLINEMETHOD detail::aggregate_without_seed_builder<TAccumulator>   aggregate (
		TAccumulator accumulator
		) throw ()
	{
		return detail::aggregate_without_seed_builder<TAccumulator> (accumulator);
	}

	template<typename TKeyPredicate>
	CPPLINQ_INLINEMETHOD detail::groupby_builder<TKeyPredicate>  groupby (TKeyPredicate key_predicate) throw ()
	{
		return detail::groupby_builder<TKeyPredicate>(std::move (key_predicate));
	}

	template<typename TKeyPredicate, typename TSelector>
	CPPLINQ_INLINEMETHOD detail::groupby_selector_builder<TKeyPredicate, TSelector>  groupby (TKeyPredicate key_predicate, TSelector selector) throw ()
	{
		return detail::groupby_selector_builder<TKeyPredicate, TSelector>(std::move (key_predicate), std::move (selector));
	}

	template<typename TKeyPredicate>
	CPPLINQ_INLINEMETHOD detail::to_groups_builder<TKeyPredicate>  to_groups (TKeyPredicate key_predicate) throw ()
	{
		return detail::to_groups_builder<TKeyPredicate>(std::move (key_predicate));
	}

	template<typename TKeyPredicate, typename TSelector>
	CPPLINQ_INLINEMETHOD detail::to_groups_selector_builder<TKeyPredicate, TSelector>  to_groups (TKeyPredicate key_predicate, TSelector selector) throw ()
	{
		return detail::to_groups_selector_builder<TKeyPredicate, TSelector>(std::move (key_predicate), std::move (selector));
	}
}
